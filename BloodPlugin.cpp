/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#include <cover/input/input.h>
#include <cover/coVRPluginSupport.h>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/ShapeDrawable>

#include "BloodPlugin.h"

#include <string>

using namespace osg;
using std::string;

BloodPlugin *BloodPlugin::plugin = NULL;

//-----------------------------------------------------------------------------

BloodPlugin::BloodPlugin()
{
    // get the parameter names (this is hardcoded)

}

bool BloodPlugin::init()
{
    fprintf(stderr, "\n    new BloodPlugin\n");
    geode = new osg::Geode();
    
    geode->setName("BloodGeometry");
    stateset = new osg::StateSet();
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    geode->setStateSet( stateset );
    
    float radius = 0.5;
    
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),radius),hints));
    
    trans = new MatrixTransform();
    trans->setName("BloodMatrixTransform");
    trans->addChild(geode.get());
    
    cover->getObjectsRoot()->addChild(trans.get());
    pos.set(0,0,0);
    v.set(1,0,0);
    
    return true;
}

BloodPlugin::~BloodPlugin()
{
    if (cover->debugLevel(1))
        fprintf(stderr, "\n    delete BloodPlugin\n");
	
    cover->getObjectsRoot()->removeChild(trans.get());

}

bool
BloodPlugin::update()
{
    pos += v*cover->frameDuration();
    osg::Matrix handMat = Input::instance()->getHandMat();
    // handMat is the transformation matrix of the 3d pointer in world coordinates

    // to transform it to the objects root coordinate system, you have to multiply it by the inverse of the base matrix
    osg::Matrix objectMat = cover->getInvBaseMat() * handMat;

    TODO:
    //compute the velocity of the hand. If the hand is faster than a threshold you could throw the sphere in that direction
    //after that you can throw multiple spheres and you can
    //ask martin how to get the status of the mouse buttons. Then you could throw blood as long as you keep the button pressed.

    //Access individual elements of transformation matrix to get change in position each frame
    double xSpeed = objectMat(0, 3);
    double ySpeed = objectMat(1, 3);
    double zSpeed = objectMat(2, 3);

    double handSpeed = sqrt(pow(xSpeed, 2) + pow(ySpeed, 2) + pow(zSpeed, 2));

    osg::Matrix mat;
    mat.makeTranslate(pos);
    trans->setMatrix(mat);
    return true;

}

COVERPLUGIN(BloodPlugin)

