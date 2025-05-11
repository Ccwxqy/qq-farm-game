# QQ Farm Game

一个用 **C++** 独立开发的农场模拟类游戏，灵感来源于早年的QQ农场。计划实现作物种植、成长、收获、买卖等功能，长期开发维护中。

---

## 🎮 当前功能规划

- [ ] 基础农田系统（土地、播种、成长、收获）
- [ ] 玩家资源系统（金币、经验）
- [ ] 商店系统（购买种子、出售作物）
- [ ] 简单图形界面（后续可能接入 SFML）
- [ ] 存档/加载机制

---

## 🛠️ 编译方式（以 macOS 为例）

'''bash
g++ -std=c++17 main.cpp -o qqfarm
./qqfarm


<details>

## 项目结构示意图（开发初期）

'''plaintext
qq-farm-game/
|--main.cpp        # 主程序入口
|--farm.cpp/.h     # 农田管理模块（待实现）
|--player.cpp/.h   # 玩家数据与逻辑
|--market.cpp/.h   # 商店逻辑
|--.vscode/        # VS Code 配置
|--README.md       # 项目说明文件
'''
</details>



> 本项目当前未设置开源协议，如需参考或使用请联系作者