// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "ColorMap.h"

#include <Core/Utility/Console.h>
#include <iostream>

namespace open3d{

namespace {

class GlobalColorMapSingleton
{
private:
    GlobalColorMapSingleton() : color_map_(new ColorMapJet) {
        PrintDebug("Global colormap init.\n");
    }
    GlobalColorMapSingleton(const GlobalColorMapSingleton &) = delete;
    GlobalColorMapSingleton &operator=(const GlobalColorMapSingleton &) = 
            delete;
public:
    ~GlobalColorMapSingleton() {
        PrintDebug("Global colormap destruct.\n");
    }

public:
    static GlobalColorMapSingleton &GetInstance() {
        static GlobalColorMapSingleton singleton;
        return singleton;
    }

public:
    std::shared_ptr<const ColorMap> color_map_;
};

class LabelColorMapSingleton
{
private:
    LabelColorMapSingleton() : color_map_(new ColorMapLabels) {
        PrintDebug("Global colormap init.\n");
    }
    LabelColorMapSingleton(const LabelColorMapSingleton &) = delete;
    GlobalColorMapSingleton &operator=(const GlobalColorMapSingleton &) = 
            delete;
public:
    ~LabelColorMapSingleton() {
        PrintDebug("Global colormap destruct.\n");
    }

public:
    static LabelColorMapSingleton &GetInstance() {
        static LabelColorMapSingleton singleton;
        return singleton;
    }

public:
    std::shared_ptr<const ColorMapLabels> color_map_;
};

}    // unnamed namespace

Eigen::Vector3d ColorMapGray::GetColor(double value) const
{
    return Eigen::Vector3d(value, value, value);
}

Eigen::Vector3d ColorMapJet::GetColor(double value) const
{
    return Eigen::Vector3d(
            JetBase(value * 2.0 - 1.5),        // red
            JetBase(value * 2.0 - 1.0),        // green
            JetBase(value * 2.0 - 0.5));    // blue
}

Eigen::Vector3d ColorMapSummer::GetColor(double value) const
{
    return Eigen::Vector3d(
            Interpolate(value, 0.0, 0.0, 1.0, 1.0),
            Interpolate(value, 0.5, 0.0, 1.0, 1.0),
            0.4);
}

Eigen::Vector3d ColorMapWinter::GetColor(double value) const
{
    return Eigen::Vector3d(
            0.0,
            Interpolate(value, 0.0, 0.0, 1.0, 1.0),
            Interpolate(value, 1.0, 0.0, 0.5, 1.0));
}

Eigen::Vector3d ColorMapHot::GetColor(double value) const
{
    Eigen::Vector3d edges[4] = {
        Eigen::Vector3d(1.0, 1.0, 1.0),
        Eigen::Vector3d(1.0, 1.0, 0.0),
        Eigen::Vector3d(1.0, 0.0, 0.0),
        Eigen::Vector3d(0.0, 0.0, 0.0),
    };
    if (value < 0.0) {
        return edges[0];
    } else if (value < 1.0 / 3.0) {
        return Interpolate(value, edges[0], 0.0, edges[1], 1.0 / 3.0);
    } else if (value < 2.0 / 3.0) {
        return Interpolate(value, edges[1], 1.0 / 3.0, edges[2], 2.0 / 3.0);
    } else if (value < 1.0) {
        return Interpolate(value, edges[2], 2.0 / 3.0, edges[3], 1.0);
    } else {
        return edges[3];
    }
}

template<typename T>
inline double F(T a) { return ((double)a)/255.0; }

ColorMapLabels::ColorMapLabels() : colors_(Eigen::MatrixXd::Zero(64,3))
{
    // fill in the colors
    // Derived in part from: http://godsnotwheregodsnot.blogspot.com/2012/09/color-distribution-methodology.html
    colors_ << 
        0,0,0,
        0,1.0,0,
        0,0,1,
        1,0,0,
        0,1,1,
        1,F(166),1.,
        1,F(219),F(102),
        0,F(100),0,
        0,0,F(103),
        F(149),0,F(58),
        0,F(125),F(181),
        1.0,0.0,F(246),
        1.0,F(238),F(232),
        F(119),F(77),0,
        F(144),F(251), F(146),
        0,F(118),1.0,
        F(213),1.0,0.0,
        1.,F(147),F(126),
        F(106),F(130),F(108),
        1.,F(2.0),F(157),
        F(254),F(137),0,
        F(122),F(71),F(130),
        F(126),F(45),F(210),
        F(133),F(169),0,
        1,0,F(86),
        F(164),F(36),0,
        0,F(174),F(126),
        F(104),F(61),F(59),
        F(189),F(198),1,
        F(38),F(52),0,
        F(189),F(211),F(147),
        0,F(185),F(23),
        F(158),0,F(142),
        0,F(21),F(68),
        F(194),F(140),F(159),
        1,F(116),F(163),
        0,F(208),1.0,
        0,F(71),F(84),
        F(229),F(111),F(254),
        F(120),F(130),F(49),
        F(14),F(76),F(161),
        F(145),F(208),F(203),
        F(190),F(153),F(112),
        F(150),F(138),F(232),
        F(187),F(136),0,
        F(67),0,F(44),
        F(222),1,F(116),
        0,1,F(210),
        1,F(229),0,
        F(98),F(14),0,
        0,F(143),F(156),
        F(152),1,F(82),
        F(117),F(68),F(177),
        F(181),0,1,
        0,1,F(120),
        1,F(110),F(65),
        0,F(95),F(57),
        F(107),F(104),F(130),
        F(95),F(173),F(78),
        F(167),F(87),F(64),
        F(165),1,F(210),
        1,F(177),F(103),
        0,F(155),1,
        F(232),F(94),F(190);
}

Eigen::Vector3d ColorMapLabels::GetColor(double v) const
{
    assert(v >= 0.0 && v <= 1.0);
    return GetColor(static_cast<uint32_t>(std::floor(v * 64.)));
}

Eigen::Vector3d ColorMapLabels::GetColor(uint32_t v) const
{
    return colors_.row(v % 64);
}

const std::shared_ptr<const ColorMap> GetGlobalColorMap()
{
    return GlobalColorMapSingleton::GetInstance().color_map_;
}

const std::shared_ptr<const ColorMapLabels> GetLabelColorMap()
{
    return LabelColorMapSingleton::GetInstance().color_map_;
}

void SetGlobalColorMap(ColorMap::ColorMapOption option)
{
    switch (option) {
    case ColorMap::ColorMapOption::Gray:
        GlobalColorMapSingleton::GetInstance().color_map_.reset(
                new ColorMapGray);
        break;
    case ColorMap::ColorMapOption::Summer:
        GlobalColorMapSingleton::GetInstance().color_map_.reset(
                new ColorMapSummer);
        break;
    case ColorMap::ColorMapOption::Winter:
        GlobalColorMapSingleton::GetInstance().color_map_.reset(
                new ColorMapWinter);
        break;
    case ColorMap::ColorMapOption::Hot:
        GlobalColorMapSingleton::GetInstance().color_map_.reset(
                new ColorMapHot);
        break;
    case ColorMap::ColorMapOption::Label:
        GlobalColorMapSingleton::GetInstance().color_map_.reset(
                new ColorMapLabels);
        break;
    case ColorMap::ColorMapOption::Jet:
    default:
        GlobalColorMapSingleton::GetInstance().color_map_.reset(
                new ColorMapJet);
        break;
    }
}

}    // namespace open3d
