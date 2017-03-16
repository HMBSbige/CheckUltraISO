# CheckUltraISO
From https://github.com/yufengzjj/PoCUltraISOKeyGen

**注意：** UltraISO有简体中文版和多语言版，本程序为多语言版的注册模拟
## UltraISO注册表位置
```
HKEY_CURRENT_USER\SOFTWARE\EasyBoot Systems\UltraISO
```

## 已知BUG

- [ ] 中文用户名KeyGen会生成特殊字符，如：
	```
	鄙哥
	4DFFC3D44BDB44B2€
	```
	但实际Key为```4DFFC3D44BDB44B2```

- [ ]  Key验证相关：

	用户名为```bige```

	Key:

	```
	8058EF0A1CC2DB8
	8058EF0A1CC2DB80
	```
	注册模拟均correct

	但显然```8058EF0A1CC2DB80```才是真Key

## VS2017编译openssl静态库
```
%comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
cd c:\openssl-*\
perl Configure VC-WIN64A
ms\do_win64a.bat
nmake -f ms\nt.mak
nmake -f ms\nt.mak test
nmake -f ms\nt.mak install
```
