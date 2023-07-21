# BQ40Z80_Display

一个基于ESP32-S2的LCD屏幕电量计信息显示模块

Working...

## 预览

![](https://github.com/acha666/BQ40Z80_Display/assets/37405625/bc1f417f-844a-4046-9cc4-c288cf4430a9)

## 基本信息

项目使用ESP32-S2主控驱动GC9A01 240x120 LCD液晶屏，通过SMBus连接BQ40Zxx电量计，读取并显示实时电压、电流等信息

基于ESP-IDF框架，液晶屏幕使用[lvgl](https://github.com/lvgl/lvgl)与[lvgl_esp32_drivers](https://github.com/lvgl/lvgl_esp32_drivers)驱动

目前仅在BQ40Z80电量计芯片上进行测试，理论上BQ40Z50芯片的大部分SBS Command与其均相同，也能支持

BQ40Z80的硬件设计可参考[acha666/BQ40Z80](https://github.com/acha666/BQ40Z80)

## 进度

- [X] 能编译
- [X] 能亮
- [X] 第一个GUI页面（电压、电流、剩余电量）
- [ ] 第二个GUI页面（每节电芯的电压、寄存器信息）
- [ ] 按钮交互
- [ ] 配套硬件模块

## 致谢

感谢[Linho](https://github.com/henrylin666)为美工苦手提供的GUI设计
