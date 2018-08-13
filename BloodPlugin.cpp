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

ParticleData::ParticleData()
{
    transformNode = new MatrixTransform();
}

ParticleData::~ParticleData()
{

}

bool BloodPlugin::init()
{
    fprintf(stderr, "\n    new BloodPlugin\n");
    geode = new osg::Geode();
    
    geode->setName("BloodGeometry");
    stateset = new osg::StateSet();
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    geode->setStateSet( stateset );
    
    float radius = 0.02;
    numOfDroplets = 0;
    
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),radius),hints));
    
    //Creates initial blood droplet
    particles.push_back(new ParticleData());

    //particles[numOfDroplets]->transformNode->setName("BloodMatrixTransform" + std::to_string(numOfDroplets));
    particles[numOfDroplets]->transformNode->addChild(geode.get());

    cover->getObjectsRoot()->addChild(particles[numOfDroplets]->transformNode.get());
    particles[numOfDroplets]->velocity.set(0, 0, 0);
    numOfDroplets++;

    previousHandPosition.set(0, 0, 0);

    gravity.set(0, 0, -9.81f);

    return true;
}

BloodPlugin::~BloodPlugin()
{
    if (cover->debugLevel(1))
        fprintf(stderr, "\n    delete BloodPlugin\n");

    //Removes all ParticleData
    for (int i = 0; i < numOfDroplets; i++)
    {
        cover->getObjectsRoot()->removeChild(particles[i]->transformNode.get());
        delete particles[i];
    }

    particles.clear();
}

bool
BloodPlugin::update()
{
    osg::Matrix handMat = cover->getPointerMat();
    // handMat is the transformation matrix of the 3d pointer in world coordinates
    // to transform it to the objects root coordinate system, you have to multiply it by the inverse of the base matrix

    //Calculate position of knife tip in world coordinates
    osg::Vec3 knifeLength(0, 300, 0);
    currentHandPosition = handMat.postMult(knifeLength);

    currentHandPosition = currentHandPosition * 100;

    if(previousHandPosition.x() == 0)
        previousHandPosition = currentHandPosition;

    //Gets the translation of the hand
    handShift = currentHandPosition - previousHandPosition;

    previousHandPosition = currentHandPosition;

    //Find change in hand position to calculate speed
    double handSpeed = handShift.length()/(cover->frameDuration());
    std::cout << handSpeed << std::endl;
    std::cout << particles[numOfDroplets - 1]->position.x() << std::endl;
    std::cout << particles[numOfDroplets - 1]->position.y() << std::endl;
    std::cout << particles[numOfDroplets - 1]->position.z() << std::endl;
    std::cout << numOfDroplets << std::endl;
    std::cout << particles[numOfDroplets - 1]->onHand << "\n\n";

    //If blood leaves the hand or current particle is not on hand
    if (handSpeed > 1 || !(particles[numOfDroplets - 1]->onHand))
    {
        //Droplet leaves hand with previous speed
        particles[numOfDroplets - 1]->velocity.set(handShift);
        particles[numOfDroplets - 1]->onHand = false;

        //New droplet is created on hand
        particles.push_back(new ParticleData());
        //particles[numOfDroplets]->transformNode->setName("BloodMatrixTransform" + std::to_string(numOfDroplets));
        particles[numOfDroplets]->transformNode->addChild(geode.get());

        cover->getObjectsRoot()->addChild(particles[numOfDroplets]->transformNode.get());
        particles[numOfDroplets]->velocity.set(0, 0, 0);
        numOfDroplets++;

        //Makes sure there are no more than ten droplets
        if (numOfDroplets > 10)
        {
            cover->getObjectsRoot()->removeChild(particles[0]->transformNode.get());
            delete particles[0];
            particles.erase(particles.begin());
            numOfDroplets--;
        }
    }
    //If blood is still on the hand
    else
    {
        particles[numOfDroplets - 1]->velocity.set(0, 0, 0);
        particles[numOfDroplets - 1]->position.set(currentHandPosition);
    }

    //y speed accelerates downward due to gravity for all other particles
    for (int i = 0; i < numOfDroplets - 1; i++)
        particles[i]->velocity += gravity * float(cover -> frameDuration());

    //Translates all droplets according to speeds
    for (int i = 0; i < numOfDroplets; i++)
        particles[i]->position += particles[i]->velocity * float(cover -> frameDuration());

    //Translate all the droplets according to matrix
    for (int i = 0; i < numOfDroplets; i++)
    {
        particles[i]->matrix.makeTranslate(particles[i]->position);
        particles[i]->transformNode->setMatrix(particles[i]->matrix);
    }

    return true;

}

COVERPLUGIN(BloodPlugin)
