//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Modified by Jonathan Schlueter 11/3/16.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// glfw includes
#include <GLFW/glfw3.h>

// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Plane3D.h"
#include "Texture.h"



using namespace std;

// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;


int main(int argc, const char * argv[]) {
    // Init the GLFW Window
    window = initWindow();
    
    // Init the glew api
    initGlew();
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
    GLAppearance* singleTexApp = new GLAppearance("../single_texture.vs", "../single_texture.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(00.0,20.0,20.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 0.0;
    light_source._diffuse_intensity = 1.0;
    light_source._attenuation_coeff = 0.0;
	singleTexApp->addLightSource(light_source);
    
    // Create a material object
    GLMaterial singleTexMat;
    singleTexMat._diffuse_material = glm::vec3(0.0, 0.0, 0.0);
    singleTexMat._ambient_material = glm::vec3(0.1, 0.1, 0.1);
    singleTexMat._specular_material = glm::vec3(1.0, 1.0, 1.0);
    singleTexMat._shininess = 12.0;
    singleTexMat._transparency = 1.0;
	singleTexApp->setMaterial(singleTexMat);
    
    
    //************************************************************************************************
    // Add a texture
    GLMultiTexture* texture = new GLMultiTexture();
	GLTexture* singleTex = new GLTexture();
	
	singleTex->loadAndCreateTexture("../isu.bmp");
	singleTexApp->setTexture(singleTex);
    
    //************************************************************************************************
    // Finalize the appearance object
	singleTexApp->finalize();
    
    
    // create the sphere geometry
    GLPlane3D* singleTexPlane = new GLPlane3D(0.0, 0.0, 0.0, 100.0, 50.0);
	singleTexPlane->setApperance(*singleTexApp);
	singleTexPlane->init();
    
    // If you want to change appearance parameters after you init the object, call the update function
	singleTexApp->updateLightSources();


    // Set up our black background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window)) {
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());
        
        // draw the objects
        cs->draw();
		singleTexPlane->draw();
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    delete cs;
}