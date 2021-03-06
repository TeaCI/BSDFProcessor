// =================================================================== //
// Copyright (C) 2014-2016 Kimura Ryo                                  //
//                                                                     //
// This Source Code Form is subject to the terms of the Mozilla Public //
// License, v. 2.0. If a copy of the MPL was not distributed with this //
// file, You can obtain one at http://mozilla.org/MPL/2.0/.            //
// =================================================================== //

#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H

#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osgGA/CameraManipulator>

#include <libbsdf/Brdf/Brdf.h>
#include <libbsdf/Common/Vector.h>

#include "MaterialData.h"
#include "SceneUtil.h"

/*!
 * \class   GraphScene
 * \brief   The GraphScene class provides the scene data for 3D graph.
 */
class GraphScene
{
public:
    GraphScene();
    ~GraphScene() {}

    enum DisplayMode {
        NORMAL_DISPLAY,
        ALL_INCOMING_POLAR_ANGLES_DISPLAY,
        ALL_INCOMING_AZIMUTHAL_ANGLES_DISPLAY,
        ALL_WAVELENGTHS_DISPLAY,
        SAMPLE_POINTS_DISPLAY,
        SAMPLE_POINT_LABELS_DISPLAY
    };

    osg::Group* getRoot() { return root_.get(); }

    osg::Group* getBsdfGroup() { return bsdfGroup_.get(); }

    void updateView(int width, int height);

    /*! Creates and adds graph geometry to the scene. */
    void createBrdfGeode();

    void createAxis(bool useTextLabel = false, bool useLogPlot = false, float baseOfLogarithm = 10.0f);

    void updateGraphGeometry(int inThetaIndex, int inPhiIndex, int wavelengthIndex);

    void updateInOutDirLine(const lb::Vec3& inDir, const lb::Vec3& outDir, int wavelengthIndex);

    void setMaterialData(MaterialData* data) { data_ = data; }

    void useOit(bool on) { useOit_ = on; }

    void useLogPlot(bool on) { useLogPlot_ = on; }
    void setBaseOfLogarithm(float base) { baseOfLogarithm_ = base; }

    void setDisplayMode(DisplayMode mode) { displayMode_ = mode; }

    inline osg::Camera* getCamera() { return camera_; }
    inline void setCamera(osg::Camera* camera) { camera_ = camera; }

    inline osgGA::CameraManipulator* getCameraManipulator() { return cameraManipulator_; }
    inline void setCameraManipulator(osgGA::CameraManipulator* manipulator) { cameraManipulator_ = manipulator; }

private:
    GraphScene(const GraphScene&);
    GraphScene& operator=(const GraphScene&);

    /*! Attachs the shader of 3D graph to a node. */
    void attachGraphShader(osg::Node* node);

    /*! Attachs the color shader to a node. */
    void attachColorShader(osg::Node* node);

    /*! Creates a post processing group. */
    osg::Group* createPostProcessing(osg::Node* subgraph, int width, int height, int numFboSamples = 4);

    void initializeInDirLine();
    void updateInDirLine(const lb::Vec3& inDir, int wavelengthIndex);
    osg::Vec3 modifyDirLineLength(const lb::Vec3& lineDir, int wavelengthIndex);
    void clearInDirLine() { inDirGeode_->removeDrawables(0, inDirGeode_->getNumDrawables()); }

    void initializeInOutDirLine();

    void updateBrdfGeometry(int inThetaIndex, int inPhiIndex, int wavelengthIndex);
    void setupBrdfMeshGeometry(lb::Brdf* brdf, float inTheta, float inPhi, int wavelengthIndex,
                               lb::DataType dataType);

    void updateSpecularReflectanceGeometry(int inThetaIndex, int inPhiIndex, int wavelengthIndex);

    void clearGraphGeometry();

    MaterialData* data_;

    osg::ref_ptr<osg::Group> root_;

    osg::ref_ptr<osg::Group> postProcessingGroup_;
    osg::ref_ptr<osg::Group> postProcessingChild_;

    osg::ref_ptr<osg::Group> oitGroup_;
    osg::ref_ptr<osg::Group> oitChild_;

    osg::ref_ptr<osg::Group> scene_;            /*!< The group for rendered geometry. */
    osg::ref_ptr<osg::Group> accessoryGroup_;   /*!< The group for accessories like axes or text labels. */

    osg::ref_ptr<osg::Group> bsdfGroup_;
    osg::ref_ptr<osg::Geode> bxdfMeshGeode_;
    osg::ref_ptr<osg::Geode> bxdfPointGeode_;
    osg::ref_ptr<osg::Geode> bxdfTextGeode_;
    osg::ref_ptr<osg::Geode> specularReflectanceGeode_;

    osg::ref_ptr<osg::Geode> axisGeode_;
    osg::ref_ptr<osg::Geode> circleGeode_;
    osg::ref_ptr<osg::Geode> axisTextLabelGeode_;
    
    osg::ref_ptr<osg::Geode> inDirGeode_;
    osg::ref_ptr<osg::Geode> inOutDirGeode_;

    int     numMultiSamples_;
    bool    useOit_;
    int     numOitPasses_;

    bool    useLogPlot_;
    float   baseOfLogarithm_;

    DisplayMode displayMode_;

    osg::Camera*                camera_;
    osgGA::CameraManipulator*   cameraManipulator_;

    /*!
     * The number of incoming directions. This is used if a coordinate system does not have
     * the angles of an incoming direction.
     */
    static const int NUM_INCOMING_POLAR_ANGLES = 19;
};

#endif // GRAPH_SCENE_H
