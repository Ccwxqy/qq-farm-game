#include "farm.h"
#include <iostream>
#include<map>
#include<cstdlib>
#include<ctime>

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
        switch (todayWeather){
            case Weather::Sunny:{
                plot.grow();
                break;
            }
            case Weather::Rainy:{
                plot.grow();
                plot.grow();
                break;
            }
            case Weather::Drought:{
                break;
            }
        }
        
    }
}

void Farm::harvestAt(int plotId, Player& player){
    if(plotId >= 0 && plotId < static_cast<int>(plots.size())){
        Plot& plot = plots[plotId];
        if(plot.hasCrop && plot.crop && plot.crop -> isMature()){
            std::string cropName = plot.crop -> name;
            auto it = cropDatabase.find(cropName);
            if(it != cropDatabase.end()){
                player.addCrop(cropName);
                player.gainExp(it -> second.sellPrice);
                std::cout<< "🌳 已经收获并放入背包中\n";
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
    std::cout << "当前等级： " << level << "   当前经验： " << experience << "\n";
}

void Player::addCrop(const std::string& name){
    inventory[name]++;
    std::cout << "🎒 背包中新增作物： " << name << " x1\n";
}

void Player::displayInventory() const{
    std::cout << "🎒 背包内容： \n";
    if(inventory.empty()){
        std::cout << "（空）\n";
        return;
    }
    for(const auto& pair : inventory){
        std::cout << "- " << pair.first << " x" << pair.second << "\n";
    }
}

void Player::sellCrop(const std::string& name, int quantity){
    auto it = inventory.find(name);
    if(it == inventory.end() || it -> second < quantity){
        std::cout << "❌ 背包中该作物数量不足！ \n";
        return;
    }

    auto cropIt = cropDatabase.find(name);
    if(cropIt == cropDatabase.end()){
        std::cout << "❌ 无法识别该作物！ \n";
        return;
    }

    int income = cropIt -> second.sellPrice * quantity;
    it -> second -= quantity;
    if(it -> second == 0){
        inventory.erase(it);
    }
    earn(income);

    std::cout << "✅ 卖出 " << name << " x" << quantity << ", 获得金币： " << income << "\n";
}

void Player::gainExp(int amount ){
    experience += amount;
    std::cout << "获得经验： " << amount << " 点\n";
    checkLevelUp();
}

void Player::checkLevelUp(){
    while(experience >= level * 50){
        experience -= level * 50;
        level++;
        std::cout << "🎉 升级！ 当前等级： " << level << "\n";
    }
}


Weather todayWeather;

void generateWeather(){
    int r = rand() % 3;
    todayWeather = static_cast<Weather>(r);

    switch (todayWeather){
        case Weather::Sunny:
            std::cout << "🌞 今日天气： 晴天（正常成长）\n";
            break;
        case Weather::Rainy:
            std::cout << "🌧️ 今日天气： 雨天（额外成长 + 1)\n";
            break;
        case Weather::Drought:
            std::cout << "☀️ 今日天气： 干旱(不成长)\n";
            break;
    }
}