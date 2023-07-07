
  puVar3 = (uint *)param_1[0x32];
  if ((((puVar3 == (uint *)0x0) || ((code *)puVar3[6] == (code *)0x0)) || (*param_1 == 0)) ||
     ((*puVar3 != uVar2 || (puVar3[1] != uVar1)))) {
    iStack_30 = (*_DAT_fffd1458)();
    if (iStack_30 != 0) {
      if (param_1[0x32] != 0) {
        (*_DAT_fffd1470)(param_1,_DAT_fffd146c);
      }
      param_1[0x32] = iStack_30;
      iVar6 = (**(code **)(iStack_30 + 0x10))(param_1);
      *param_1 = iVar6;
      if (iVar6 != 0) {
        if (*(code **)(param_1[0x32] + 0x18) == (code *)0x0) {
          iVar5 = 0;
        }
        else {
          iVar5 = (**(code **)(param_1[0x32] + 0x18))(param_1,iVar6,auStack_40,param_2);
        }
        goto LAB_000114b6;
      }
      (*_DAT_fffd148c)(1,_DAT_fffd1488,_DAT_fffd1484);
      param_1[0x32] = 0;
    }
    iVar5 = (*_DAT_fffd1500)(param_1,uVar7,*(undefined *)(iVar5 + 1));
    if (iVar5 == 0) {
      return -1;
    }
  }
  else {
    iVar5 = (*(code *)puVar3[6])(param_1,*param_1,auStack_40,param_2);
LAB_000114b6:
    pcVar4 = *(code **)(param_1[0x32] + 0x1c);
    if (((pcVar4 != (code *)0x0) && (*(int *)(param_1[0x32] + 0x20) != 0)) &&
       (iVar6 = (*pcVar4)(param_1,*param_1), iVar6 != 0)) {
      if (param_1[0x2f] != 0) {
        (*_DAT_fffd14d8)(param_1[0x2f],_DAT_fffd14d4,0x1d8);
      }
      iVar6 = (**(code **)(param_1[0x32] + 0x20))(param_1,*param_1,param_1 + 0x30);
      param_1[0x2f] = iVar6;
    }
  }
LAB_0001150a:
  if (iVar5 == 0) {
    (*_DAT_fffd1514)(1,_DAT_fffd1510,_DAT_fffd150c);
    iVar6 = -1;
  }
  else {
    iVar6 = (*_DAT_fffd1528)(param_1,iVar5);
    if (iVar6 == 0) {
      if (param_1[0x31] != iVar5) {
        (*_DAT_fffd1538)();
        param_1[0x31] = iVar5;
      }
    }
    else {
      (*_DAT_fffd1548)(param_1[0x31]);
      param_1[0x31] = 0;
      (*_DAT_fffd1554)(iVar5);
      if (iVar6 == -2) {
        (*_DAT_fffd1564)(3);
        iVar6 = 3;
      }
    }
  }
  return iVar6;
}
