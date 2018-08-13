/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef BLOOD_PLUGIN_H
#define BLOOD_PLUGIN_H

#include <cover/coVRPlugin.h>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <vector>

using namespace opencover;

class ParticleData
{

public:
    ParticleData();
    ~ParticleData();
    void moveForward();

    osg::Vec3 velocity;
    osg::Vec3 position;
    osg::ref_ptr<osg::MatrixTransform> transformNode;
    osg::Matrix matrix;
    bool onHand = true;
};

class BloodPlugin : public coVRPlugin
{
private:

public:
    static BloodPlugin *plugin;

    //Global pointers for plugin objects
    osg::ref_ptr<osg::StateSet> stateset;
    osg::ref_ptr<osg::Material> linemtl;
    osg::ref_ptr<osg::Geometry> geom;
    osg::ref_ptr<osg::Geode> geode;

    std::vector <ParticleData*> particles;

    int numOfDroplets;

    //Useful velocity variables
    osg::Vec3 currentHandPosition;
    osg::Vec3 previousHandPosition;

    osg::Vec3 handShift;
    
    osg::Vec3 gravity;

    //Plugin constructor
    BloodPlugin();

    //Plugin destructor
    virtual ~BloodPlugin();

    //Plugin initializer
    bool init();

    //Called every frame load
    bool update();
};

#endif
