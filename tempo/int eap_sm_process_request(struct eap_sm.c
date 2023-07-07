
    if (sm->m && sm->m->process && sm->eap_method_priv &&
            reqVendor == sm->m->vendor &&
            reqVendorMethod == sm->m->method) {
        resp = sm->m->process(sm, sm->eap_method_priv,
                              &m_res, reqData);
    } else {
        m = eap_peer_get_eap_method(reqVendor, reqVendorMethod);
        if (m == NULL) {
            goto build_nak;
        }
        if (sm->m) {
            eap_deinit_prev_method(sm, "GET_METHOD");
        }
        sm->m = m;
        sm->eap_method_priv = sm->m->init(sm);
        if (sm->eap_method_priv == NULL) {
            wpa_printf(MSG_ERROR, "Method private structure allocated failure\n");
            sm->m = NULL;
            goto build_nak;
        }

        if (sm->m->process) {
            resp = sm->m->process(sm, sm->eap_method_priv, &m_res, reqData);
        }
    }

    if (sm->m->isKeyAvailable && sm->m->getKey &&
            sm->m->isKeyAvailable(sm, sm->eap_method_priv)) {
        if (sm->eapKeyData) {
            os_free(sm->eapKeyData);
        }
        sm->eapKeyData = sm->m->getKey(sm, sm->eap_method_priv,
                                       &sm->eapKeyDataLen);
    }
    goto send_resp;

build_nak:
    resp = (struct wpabuf *)eap_sm_build_nak(sm, type, ehdr->identifier);
    if (resp == NULL) {
        return ESP_FAIL;
    }
    ret = ESP_FAIL;

send_resp:
    if (resp == NULL) {
        wpa_printf(MSG_ERROR, "Response build fail, return.");
        return ESP_FAIL;
    }
    ret = eap_sm_send_eapol(sm, resp);
    if (ret == ESP_OK) {
        if (resp != sm->lastRespData) {
            wpabuf_free(sm->lastRespData);
            sm->lastRespData = resp;
        }
    } else {
        wpabuf_free(sm->lastRespData);
        sm->lastRespData = NULL;
        wpabuf_free(resp);
        resp = NULL;

        if (ret == WPA_ERR_INVALID_BSSID) {
            ret = WPA2_ENT_EAP_STATE_FAIL;
            wpa2_set_eap_state(WPA2_ENT_EAP_STATE_FAIL);
        }
    }
out:
    return ret;
}
