
int eap_sm_process_request(int *param_1,int reqData)

{
  char cVar1;
  uint uVar2;
  uint *puVar3;
  uint uVar4;
  int ret;
  int resp;
  uint uVar7;
  code *pcVar8;
  undefined auStack_30 [16];
  uint uStack_20;
  int iStack_1c;
  
  if (reqData == 0) {
    return -1;
  }
  if (*(uint *)(reqData + 4) < 4) {
    return -1;
  }
  resp = *(int *)(reqData + 8);
  if (*(int *)(reqData + 8) == 0) {
    resp = reqData + 0xc;
  }
  uVar4 = (*(ushort *)(resp + 2) & 0xff) << 8 | (uint)(*(ushort *)(resp + 2) >> 8);
  if (*(uint *)(reqData + 4) < uVar4) {
    return -1;
  }
  cVar1 = *(char *)(resp + 1);
  if (*(char *)(param_1 + 0x2a) == cVar1) {
    resp = param_1[0x30];
    goto send_resp;
  }
  *(char *)(param_1 + 0x2a) = cVar1;
  uVar2 = (uint)*(byte *)(resp + 4);
  if (uVar2 == 1) {
    resp = (*(code *)0x0)(param_1,cVar1,0);
    goto send_resp;
  }
  if (uVar2 == 2) {
    return 0;
  }
  if (uVar2 == 0xfe) {
    if (uVar4 < 0xc) {
      return -1;
    }
    uVar7 = (uint)*(byte *)(resp + 7) |
            (uint)*(byte *)(resp + 5) << 0x10 | (uint)*(byte *)(resp + 6) << 8;
    uVar4 = (uint)CONCAT11(*(undefined *)(resp + 10),*(undefined *)(resp + 0xb)) |
            (uint)*(byte *)(resp + 8) << 0x18 | (uint)*(byte *)(resp + 9) << 0x10;
  }
  else {
    uVar7 = 0;
    uVar4 = uVar2;
  }
  puVar3 = (uint *)param_1[0x31];
  uStack_20 = uVar2;
  if ((((puVar3 == (uint *)0x0) || (*param_1 == 0)) || (*puVar3 != uVar7)) || (puVar3[1] != uVar4) )
  {
    ret = (*(code *)0x0)();
    if (ret != 0) {
      iStack_1c = ret;
      if (param_1[0x31] != 0) {
        (*(code *)0x0)(param_1,0);
      }
      param_1[0x31] = iStack_1c;
      ret = (**(code **)(iStack_1c + 0x10))(param_1);
      *param_1 = ret;
      if (ret != 0) {
        resp = (**(code **)(param_1[0x31] + 0x18))(param_1,ret,auStack_30,reqData);
        goto build_nak;
      }
      (*(code *)0x0)(0x90);
      param_1[0x31] = 0;
    }
    resp = (*(code *)&gl_ca_cert_len)(param_1,uStack_20,*(undefined *)(resp + 1));
  }
  else {
    resp = (*(code *)puVar3[6])(param_1,*param_1,auStack_30,reqData);
build_nak:
    pcVar8 = *(code **)(param_1[0x31] + 0x1c);
    if (((pcVar8 != (code *)0x0) && (*(int *)(param_1[0x31] + 0x20) != 0)) &&
       (ret = (*pcVar8)(param_1,*param_1), ret != 0)) {
      if (param_1[0x2e] != 0) {
        (*(code *)0x0)(param_1[0x2e],0,0x243);
        param_1[0x2e] = 0;
      }
      ret = (**(code **)(param_1[0x31] + 0x20))(param_1,*param_1,param_1 + 0x2f);
      param_1[0x2e] = ret;
    }
  }
send_resp:
  if (resp == 0) {
    return -1;
  }
  ret = (*(code *)0x0)(param_1,resp);
  if (ret == 0) {
    if (param_1[0x30] != resp) {
      (*(code *)0x0)();
      param_1[0x30] = resp;
      return 0;
    }
    return 0;
  }
  (*(code *)0x0)();
  param_1[0x30] = 0;
  (*(code *)0x0)(resp);
  if (ret == -4) {
    (*(code *)&gl_anonymous_identity)(3);
    return 3;
  }
  return ret;
}
