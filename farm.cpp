#include "farm.h"
#include <iostream>
#include<map>

const std::map<std::string, CropInfo> cropDatabase = {
    {"玉米", {"玉米", 3, 10, 25}},
    {"番茄", {"番茄", 4, 15, 30}},
    {"西瓜", {"西瓜", 5, 25, 50}}
};

// ===== Crop 实现 =====
Crop::Crop(const std::string& name, int required): name(name), growth(0), growth_required(required){

}

bool Crop::isMature() const{
    return growth >= growth_required;
}

// ==== Plot 实现 ====
Plot::Plot(int id): id(id), hasCrop(false), crop(nullptr){

}

void Plot::plantCrop(const std::string& cropName){
    if(hasCrop){
        std::cout << "田块" << id << "已有作物，不能重复种植。 \n";
        return;
    }
    crop = std::make_unique<Crop>(cropName, 3); //成熟需要3轮
    hasCrop = true;
    std::cout << "田块" << id << "种下了： " << cropName << std::endl;
}

void Plot::plantCrop(const std::string& cropName, int requiredGrowDays){
    if(hasCrop){
        std::cout<< "田块 " << id << " 已有作物，不能重复种植。 \n";
        return;
    }
    crop = std::make_unique<Crop>(cropName, requiredGrowDays);
    hasCrop = true;
    std::cout<< "田块 " << id << " 种下了： " << cropName << std::endl;
}

void Plot::grow(){
    if(hasCrop && crop){
        crop -> growth++;
    }
}

void Plot::harvest(){
    if(hasCrop && crop){
        if(crop -> isMature()){
            std::cout << "✅ 成功收获田块：" << id << "的作物： " << crop -> name << std::endl;
            crop.reset();  //自动释放内存
            hasCrop = false;
        }else{
            std::cout << "⚠️ 作物还未成熟，无法收获。 \n";
        }
    }else{
        std::cout << "田块 " << id << " 没有作物可收获。 \n";
    }
}

void Plot::displayStatus() const{
    std::cout << "田块 " << id << ": ";
    if(hasCrop && crop){
        std::cout << crop -> name << "(生长 " << crop -> growth << "/" << crop -> growth_required << ")";
        if(crop -> isMature()){
            std::cout << " ✅";
        }
        std::cout << "\n";
    }else{
        std::cout << "空闲 \n";
    }
}

// ==== Farm 实现 =====
Farm::Farm(int size){
    for(int i = 0; i < size; ++i){
        plots.emplace_back(i);
    }
}

void Farm::plantAt(int plotId, const std::string& cropName, Player& player){
    if(plotId >= 0 && plotId < static_cast<int>(plots.size())){
        auto it = cropDatabase.find(cropName);
        if(it == cropDatabase.end()){
            std::cout<< "❌ 商店中没有这种作物： "<< cropName << " \n";
            return;
        }

        int cost = it -> second.cost;
        if(!player.canAfford(cost)){
            std::cout<< "❌ 金币不足，无法种植：" << cropName << "\n";
            return;
        }
        plots[plotId].plantCrop(cropName, it -> second.growDays);   //重载 playtCrop
        player.spend(cost);
        std::cout << "💰 花费 " << cost << " 元种下了 " << cropName << "\n";
    }else{
        std::cout << "❌ 无效的田块编号 \n";
    }
}

void Farm::growAll(){
    for(auto& plot : plots){
        plot.grow();
    }
}

void Farm::harvestAt(int plotId, Player& player){
    if(plotId >= 0 && plotId < static_cast<int>(plots.size())){
        Plot& plot = plots[plotId];
        if(plot.hasCrop && plot.crop && plot.crop -> isMature()){
            std::string cropName = plot.crop -> name;
            auto it = cropDatabase.find(cropName);
            if(it != cropDatabase.end()){
                int value = it -> second.sellPrice;
                player.earn(value);
                std::cout<< "💰 收获后获得收益：" << value << " 元\n";
            }
        }
        plot.harvest(); // 自动清空
        
    }else{
        std::cout << "❌ 无效的田块编号 \n";
    }
}
void Farm::displayFarm() const{
    std::cout << "\n📄 当前农田状态： \n";
    for(const auto& plot : plots){
        plot.displayStatus();
    }
}

Player::Player(int startingCoins): coins(startingCoins){}

bool Player::canAfford(int amount) const{
    return coins >= amount;
}

void Player::spend(int amount){
    if(canAfford(amount)) coins -= amount;
}

void Player::earn(int amount){
    coins += amount;
}

void Player::displayStatus() const{
    std::cout<< "💰 当前金币： "<< coins <<" 元\n";
}