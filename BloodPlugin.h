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


using namespace opencover;

class BloodPlugin : public coVRPlugin
{
private:

public:
    static BloodPlugin *plugin;


    osg::ref_ptr<osg::StateSet> stateset;
    osg::ref_ptr<osg::Material> linemtl;
    osg::ref_ptr<osg::Geometry> geom;
    osg::ref_ptr<osg::Geode> geode;
    osg::ref_ptr<osg::MatrixTransform> trans;
    osg::Vec3 v;
    osg::Vec3 pos;
    
    BloodPlugin();
    virtual ~BloodPlugin();
    bool init();
    bool update();
};

#endif