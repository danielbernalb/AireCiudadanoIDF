int eap_sm_process_request(struct eap_sm *sm, struct wpabuf *reqData)
{
    size_t plen;
    u32 reqVendor, reqVendorMethod;
    u8 type, *pos;
    struct eap_hdr *ehdr;
    const struct eap_method *m = NULL;
    struct wpabuf *resp = NULL;
    struct eap_method_ret m_res;
    int ret = 0;

    if (reqData == NULL || wpabuf_len(reqData) < sizeof(*ehdr)) {
        return ESP_ERR_INVALID_ARG;
    }

    ehdr = (struct eap_hdr *)wpabuf_head(reqData);
    plen = be_to_host16(ehdr->length);
    if (plen > wpabuf_len(reqData)) {
        return ESP_FAIL;
    }

    if (ehdr->identifier == sm->current_identifier) {
        /*Retransmit*/
        resp = sm->lastRespData;
        goto send_resp;
    }

    sm->current_identifier = ehdr->identifier;

    pos = (u8 *)(ehdr + 1);
    type = *pos++;
    if (type == EAP_TYPE_IDENTITY) {
        resp = (struct wpabuf *)eap_sm_build_identity_resp(sm, ehdr->identifier, 0);
        goto send_resp;
    } else if (type == EAP_TYPE_NOTIFICATION) {
        /*Ignore*/
        goto out;
    } else if (type == EAP_TYPE_EXPANDED) {
        if (plen < sizeof(*ehdr) + 8) {
            return ESP_FAIL;
        }
        reqVendor = WPA_GET_BE24(pos);
        pos += 3;
        reqVendorMethod = WPA_GET_BE32(pos);
    } else {
        reqVendor = EAP_VENDOR_IETF;
        reqVendorMethod = type;
    }

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
