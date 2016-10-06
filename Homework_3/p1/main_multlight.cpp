//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//
//	Modified by Jonathan Schlueter on 10/5/16.
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
#include "Sphere3D.h"
#include "GLAppearance.h"




using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;



int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);

    
    // Create appearance objects for each unique sphere

	GLAppearance* sp1App = new GLAppearance("../../../data/shaders/multi_vertex_lights.vs", "../../../data/shaders/multi_vertex_lights.fs");
    GLAppearance* sp2App = new GLAppearance("../../../data/shaders/multi_vertex_lights.vs", "../../../data/shaders/multi_vertex_lights.fs");
	GLAppearance* sp3App = new GLAppearance("../../../data/shaders/multi_vertex_lights.vs", "../../../data/shaders/multi_vertex_lights.fs");
	GLAppearance* sp4App = new GLAppearance("../../../data/shaders/multi_vertex_lights.vs", "../../../data/shaders/multi_vertex_lights.fs");

	// Create light sources for each sphere
	GLDirectLightSource  sp1_ls1;
	sp1_ls1._lightPos = glm::vec4(20.0, 0.0, 0.0, 0.0);
	sp1_ls1._ambient_intensity = 0.01;
	sp1_ls1._specular_intensity = 0.5;
	sp1_ls1._diffuse_intensity = 0.5;
	sp1_ls1._attenuation_coeff = 0.02;
    
    GLDirectLightSource  sp2_ls1;
    sp2_ls1._lightPos = glm::vec4(20.0, 0.0, 0.0, 0.0);
    sp2_ls1._ambient_intensity = 0.01;
    sp2_ls1._specular_intensity = 0.0;
    sp2_ls1._diffuse_intensity = 0.5;
    sp2_ls1._attenuation_coeff = 0.02;

	GLSpotLightSource  sp3_ls1;
	sp3_ls1._lightPos = glm::vec4(2.0, 10.0, -4.0, 1.0);
	sp3_ls1._ambient_intensity = 0.0;
	sp3_ls1._specular_intensity = 0.0;
	sp3_ls1._diffuse_intensity = 1000.0;
	sp3_ls1._attenuation_coeff = 0.0;
	sp3_ls1._cone_angle = 45.0;
	sp3_ls1._cone_direction = glm::vec3(0.0, -1.0, 0.0);

	GLDirectLightSource  sp3_ls2;
	sp3_ls2._lightPos = glm::vec4(10.0, 10.0, 0.0, 0.0);
	sp3_ls2._ambient_intensity = 0.0;
	sp3_ls2._specular_intensity = 2.0;
	sp3_ls2._diffuse_intensity = 0.0;
	sp3_ls2._attenuation_coeff = 0.0;

	GLSpotLightSource  sp4_ls1;
	sp4_ls1._lightPos = glm::vec4(2.0, 10.0, -12.0, 1.0);
	sp4_ls1._ambient_intensity = 0.1;
	sp4_ls1._specular_intensity = 0.0;
	sp4_ls1._diffuse_intensity = 1.8;
	sp4_ls1._attenuation_coeff = 0.01;
	sp4_ls1._cone_angle = 35.0;
	sp4_ls1._cone_direction = glm::vec3(0.0, -1.0, 0.0);

	GLDirectLightSource  sp4_ls2;
	sp4_ls2._lightPos = glm::vec4(10.0, 10.0, -4.0, 0.0);
	sp4_ls2._ambient_intensity = 0.0;
	sp4_ls2._specular_intensity = 0.1;
	sp4_ls2._diffuse_intensity = 0.0;
	sp4_ls2._attenuation_coeff = 0.0;

	GLDirectLightSource  sp4_ls3;
	sp4_ls3._lightPos = glm::vec4(10.0, 0.0, 5.0, 0.0);
	sp4_ls3._ambient_intensity = 0.0;
	sp4_ls3._specular_intensity = 0.1;
	sp4_ls3._diffuse_intensity = 0.0;
	sp4_ls3._attenuation_coeff = 0.0;
    
    
    // add lights to each sphere's appearance
    sp1App->addLightSource(sp1_ls1);

	sp2App->addLightSource(sp2_ls1);

	sp3App->addLightSource(sp3_ls1);
	sp3App->addLightSource(sp3_ls2);

	sp4App->addLightSource(sp4_ls1);
	sp4App->addLightSource(sp4_ls2);
	sp4App->addLightSource(sp4_ls3);


    // Create materials for each sphere
    GLMaterial sp1Mat;
    sp1Mat._diffuse_material = glm::vec3(1.0, 0.0, 0.0);
    sp1Mat._ambient_material = glm::vec3(1.0, 0.0, 0.0);
    sp1Mat._specular_material = glm::vec3(1.0, 1.0, 1.0);
    sp1Mat._shininess = 120.0;

	GLMaterial sp2Mat;
	sp2Mat._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
	sp2Mat._ambient_material = glm::vec3(0.0, 0.0, 1.0);
	sp2Mat._specular_material = glm::vec3(1.0, 1.0, 1.0);
	sp2Mat._shininess = 120.0;

	GLMaterial sp3Mat;
	sp3Mat._diffuse_material = glm::vec3(0.0, 1.0, 0.0);
	sp3Mat._ambient_material = glm::vec3(0.0, 1.0, 0.0);
	sp3Mat._specular_material = glm::vec3(1.0, 1.0, 1.0);
	sp3Mat._shininess = 120.0;

	GLMaterial sp4Mat;
	sp4Mat._diffuse_material = glm::vec3(1.0, 1.0, 0.2);
	sp4Mat._ambient_material = glm::vec3(1.0, 1.0, 0.2);
	sp4Mat._specular_material = glm::vec3(1.0, 1.0, 1.0);
	sp4Mat._shininess = 120.0;
    
    // Add materials to the apperance objects
    sp1App->setMaterial(sp1Mat);
    sp1App->finalize();

	sp2App->setMaterial(sp2Mat);
	sp2App->finalize();

	sp3App->setMaterial(sp3Mat);
	sp3App->finalize();

	sp4App->setMaterial(sp4Mat);
	sp4App->finalize();
    
    // Create the sphere geometry
    GLSphere3D* sp1 = new GLSphere3D(0.0, 0.0, 12.0, 4.0, 180, 100);
    sp1->setApperance(*sp1App);
    sp1->init();

	GLSphere3D* sp2 = new GLSphere3D(0.0, 0.0, 4.0, 4.0, 180, 100);
	sp2->setApperance(*sp2App);
	sp2->init();

	GLSphere3D* sp3 = new GLSphere3D(0.0, 0.0, -4.0, 4.0, 180, 100);
	sp3->setApperance(*sp3App);
	sp3->init();

	GLSphere3D* sp4 = new GLSphere3D(0.0, 0.0, -12.0, 4.0, 180, 100);
	sp4->setApperance(*sp4App);
	sp4->init();
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 35.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
   // sphere->enableNormalVectorRenderer();
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());
        
        // draw the objects
        cs->draw();
        
        sp1->draw();
		sp2->draw();
		sp3->draw();
		sp4->draw();
        
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

