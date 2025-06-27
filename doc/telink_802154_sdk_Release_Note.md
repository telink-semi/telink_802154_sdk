## V1.4.0.0


### Version
* Chips
  - TC32: TLSR8269/TLSR8258/TLSR8278
  - RISCV: TLSR951x(B91)
* Hardware Version
  - TLSR8258: C1T139A30_V1_2/C1T139A3_V2.0A
  - TLSR951x(B91): C1T213A20_V1_3/C1T213A3_V1_1
* Driver Version
  - tl_platform_sdk V3.5.0
* Toolchain Version
  - TLSR8269/TLSR8258/TLSR8278: TC32 ELF GCC4.3
  - TLSR951x(B91): TL32 ELF MCULIB V5F GCC7.4

### Features
* update the driver code of the tlsr8 and tlsr9 platform.
* add apis to protect the flash from accidental erasing or writing.
### Bug Fixes
* modify the config to ensure that both Telink IoT Studio and Telink IDE 1.3 can be compiled.
* correct some files that contain misspellings.
### BREAKING CHANGES
* N/A

### CodeSize
* Units: KBytes

|   chip   | coordinator(RAM/FLASH Size) | end device(RAM/FLASH Size) | 
| :------- | :-----------------------: | :--------------------------: |
| TLSR826x | 34 / 176                  | 32 / 196                     |
| TLSR8278 |  - / -                    |  - / -                       |
| TLSR8258 | 36 / 161                  | 34 / 180                     |
| TLSR9518 |  - / -                    |  - / -                       |



### Features
* 更新tlsr8, tlsr9的驱动代码.
* 添加函数接口(api)，以防止flash被意外擦写.


### Bug Fixes
* 修改工程配置, 确保SDK在Telink IoT Studio 和 Telink IDE 1.3上都可以编译.
* 更正一些包含拼写错误的文件.
### BREAKING CHANGES
* N/A



## V1.3.0.3


### Features
* N/A
### Bug Fixes
* fixed multi-devive and coordinator communication issues
* correct some files that contain misspellings.
### BREAKING CHANGES
* N/A

### Features
* N/A
### Bug Fixes
* 修复多设备和协调器的通讯问题.
* 更正一些包含拼写错误的文件.
### BREAKING CHANGES
* N/A



## V1.3.0.2


### Features
* Normalizes the naming of content related to private schemas
### Bug Fixes
* N/A
### BREAKING CHANGES
* N/A

### Features
* 规范一些私有条目的命名
### Bug Fixes
* N/A
### BREAKING CHANGES
* N/A



## V1.3.0.1


### Features

* support 9518 chip
* optimize ota demo

### Bug Fixes

* N/A

### BREAKING CHANGES

* N/A

### Features

* 支持9518
* 优化OTA示例

### Bug Fixes

* N/A

### BREAKING CHANGES

* N/A


## V1.3.0.0


### Features

* support 826x/8258/8278 chips
* support two roles in the demo:coordinator and end device
* ota demo

### Bug Fixes

* N/A

### BREAKING CHANGES

* N/A

### Features

* 支持826x/8258/8278
* 示例中支持协调器和终端节点两个角色
* OTA示例

### Bug Fixes

* N/A

### BREAKING CHANGES

* N/A