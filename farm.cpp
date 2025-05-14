#include "farm.h"
#include <iostream>

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

void Farm::plantAt(int plotId, const std::string& cropName){
    if(plotId >= 0 && plotId < static_cast<int>(plots.size())){
        plots[plotId].plantCrop(cropName);
    }else{
        std::cout << "❌ 无效的田块编号 \n";
    }
}

void Farm::growAll(){
    for(auto& plot : plots){
        plot.grow();
    }
}

void Farm::harvestAt(int plotId){
    if(plotId >= 0 && plotId < static_cast<int>(plots.size())){
        plots[plotId].harvest();
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