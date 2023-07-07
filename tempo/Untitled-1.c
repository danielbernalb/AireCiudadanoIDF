  
  
  puVar3 = (uint *)param_1[0x31];
  uStack_20 = uVar2;
  if ((((puVar3 == (uint *)0x0) || (*param_1 == 0)) || (*puVar3 != uVar7)) || (puVar3[1] != uVar4) )
  {
    iVar5 = (*(code *)0x0)();
    if (iVar5 != 0) {
      iStack_1c = iVar5;
      if (param_1[0x31] != 0) {
        (*(code *)0x0)(param_1,0);
      }
      param_1[0x31] = iStack_1c;
      iVar5 = (**(code **)(iStack_1c + 0x10))(param_1);
      *param_1 = iVar5;
      if (iVar5 != 0) {
        iVar6 = (**(code **)(param_1[0x31] + 0x18))(param_1,iVar5,auStack_30,param_2);
        goto LAB_00010ab7;
      }
      (*(code *)0x0)(0x90);
      param_1[0x31] = 0;
    }
    iVar6 = (*(code *)&gl_ca_cert_len)(param_1,uStack_20,*(undefined *)(iVar6 + 1));
  }
  else {
    iVar6 = (*(code *)puVar3[6])(param_1,*param_1,auStack_30,param_2);
LAB_00010ab7:
    pcVar8 = *(code **)(param_1[0x31] + 0x1c);
    if (((pcVar8 != (code *)0x0) && (*(int *)(param_1[0x31] + 0x20) != 0)) &&
       (iVar5 = (*pcVar8)(param_1,*param_1), iVar5 != 0)) {
      if (param_1[0x2e] != 0) {
        (*(code *)0x0)(param_1[0x2e],0,0x243);
        param_1[0x2e] = 0;
      }
      iVar5 = (**(code **)(param_1[0x31] + 0x20))(param_1,*param_1,param_1 + 0x2f);
      param_1[0x2e] = iVar5;
    }
  }
joined_r0x000109b7:
  if (iVar6 == 0) {
    return -1;
  }
  iVar5 = (*(code *)0x0)(param_1,iVar6);
  if (iVar5 == 0) {
    if (param_1[0x30] != iVar6) {
      (*(code *)0x0)();
      param_1[0x30] = iVar6;
      return 0;
    }
    return 0;
  }
  (*(code *)0x0)();
  param_1[0x30] = 0;
  (*(code *)0x0)(iVar6);
  if (iVar5 == -4) {
    (*(code *)&gl_anonymous_identity)(3);
    return 3;
  }
  return iVar5;
}
