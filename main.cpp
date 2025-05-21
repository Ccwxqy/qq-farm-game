#include "farm.h"
#include<iostream>
#include<string>

void showMenu(){
    std::cout<<  "\n ==== QQ 农场菜单 ====\n";
    std::cout<<  "1. 查看农田\n";
    std::cout<<  "2. 播种作物\n";
    std::cout<<  "3. 跳过一天（作物成长）\n";
    std::cout<<  "4. 收获作物\n";
    std::cout<<  "5. 查看金币\n";
    std::cout<<  "6. 退出游戏\n";
    std::cout<<  "请选择操作编号： ";

}

int main(){
    Player player(100);
    Farm farm(3);

    std::cout<< "🎮 欢迎来到 QQ 农场游戏！ 初始金币： 100\n";

    bool running = true;
    while(running){
        showMenu();

        int choice;
        std::cin >> choice;
        
        switch(choice){
            case 1: {
                farm.displayFarm();
                break;
            }
            case 2: {
                int plotId;
                std::string cropName;

                std::cout << "输入田块编号（0-" << 2 << "): ";
                std::cin >> plotId;
                std::cout << "输入作物名称（例如： 玉米、 番茄、 西瓜）：";
                std::cin >> cropName;

                farm.plantAt(plotId, cropName, player);
                break;
            }
            case 3: {
                farm.growAll();
                std::cout << "🌞 所有作物成长了一天！ \n";
                break;
            }
            case 4: {
                int plotId;
                std::cout << "输入要收获的田块编号： ";
                std::cin >> plotId;
                farm.harvestAt(plotId, player);
                break;
            }
            case 5: {
                player.displayStatus();
                break;
            }
            case 6: {
                std::cout << "👏 游戏结束，再见！ \n";
                running = false;
                break;
            }
            default:
                std::cout << "❌ 无效输入， 请重新选择。 \n";
        }
    }
    return 0;
}