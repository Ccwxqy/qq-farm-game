#ifndef FARM_H
#define FARM_H

#include<string>
#include<vector>
#include<memory>

//作物类：目前简化为名称 + 是否成熟
class Crop{
    public:
        std::string name;
        int growth; //生长值（简单模拟成长过程）
        int growth_required;

        Crop(const std::string& name, int required);
        bool isMature() const;
};

//每个田块
class Plot{
    public:
        int id;
        bool hasCrop;
        std::unique_ptr<Crop> crop;

        Plot(int id);
        void plantCrop(const std::string& cropName);
        void grow();    //增长一次
        void harvest(); //收获作物
        void displayStatus() const;
};

//整个农田
class Farm{
    private:
        std::vector<Plot> plots;
    
    public:
        Farm(int size);//初始化指定数量田块
        void plantAt(int plotId, const std::string& cropName);
        void growAll();
        void harvestAt(int plotId);
        void displayFarm() const;
};




#endif