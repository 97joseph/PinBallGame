#include "graphics.h"

Graphics::Graphics()
{
	delay = 1;
	frameCount = 1;
	fps = 1;
}

Graphics::~Graphics()
{
	Object::end();
}
//
bool Graphics::InitShader(Shader *&shader, string vertex, string fragment)
{
	 // Set up the shaders
	  shader = new Shader();
	  if(!shader->Initialize())
	  {
	    printf("Shader Failed to Initialize\n");
	    return false;
	  }

	  // Add the vertex shader
	  if(!shader->AddShader(GL_VERTEX_SHADER, vertex))
	  {
	    printf("Vertex Shader failed to Initialize\n");
	    return false;
	  }

	  // Add the fragment shader
	  if(!shader->AddShader(GL_FRAGMENT_SHADER, fragment))
	  {
	    printf("Fragment Shader failed to Initialize\n");
	    return false;
	  }

	  // Connect the program
	  if(!shader->Finalize())
	  {
	    printf("Program to Finalize\n");
	    return false;
	  }
	  return true;
}

bool Graphics::Initialize(int width, int height, SDL_Window *window)
{
	this->width = width;
	this->height = height;
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  //init UI
  ui.initGUI(window);

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //initialize fbo
  generateFrameBuffer(fbo, fbTex, width, height);

  //allocate depth and stencil buffers for base target
  GLuint rboDepthStencil;
  glGenRenderbuffers(1, &rboDepthStencil);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
  );

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
	  auto error = glGetError();
	  string val = ErrorString( error );
	  std::cerr << "Framebuffer creation was not successful:"
			  << endl << error << " "<< val << endl;
  }
  //reset to default buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //initialize quad
  glGenBuffers(1, &screen);
  glBindBuffer(GL_ARRAY_BUFFER, screen);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Init Camera
  m_camera = new camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //create shader
  if (!InitShader(m_shader, "shaders/phongFrag.vsh", "shaders/phongFrag.fsh"))
	  return false;
  if (!InitShader(m_vshader, "shaders/phongVert.vsh", "shaders/phongVert.fsh"))
	  return false;
  if (!InitShader(m_screenShader, "shaders/screenShader.vsh", "shaders/screenShader.fsh"))
	  return false;
  selected = m_shader;
  //initialize object default directory
  Object::init();
  Shader::init();



  // Create the object

  world = new World();
  world->Begin();

  world->setLightPointer(
		  m_shader->GetUniformLocation("lPos"),
		  m_shader->GetUniformLocation("lRad"),
		  m_shader->GetUniformLocation("lSize"),
		  m_shader->GetUniformLocation("lCol"),
			m_shader->GetUniformLocation("atmosphere")
	);


  //set world for camera

  m_camera->SetWorld(world);

	cout << "World created successfully" << endl;

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }
  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

	cout << "Graphics initialized" << endl;
  return true;
}

void Graphics::generateFrameBuffer(GLuint &fbo, GLuint &fbTarget, int width, int height)
{
	  glGenFramebuffers(1, &fbo);
	  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	  //allocate texture
	  glGenTextures(1, &fbTarget);
	  glBindTexture(GL_TEXTURE_2D, fbTarget);

	  glTexImage2D(
	      GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	  );

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	  glFramebufferTexture2D(
	      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTarget, 0
	  );
}

void Graphics::Update(unsigned int dt)
{
		  //get keyboard
		  for (int i = 0; i < listener.getSize(); i++)
		  {
			if (listener.getEvent(i).eventVer == SDL_KEYDOWN)
				{
					if (listener.getEvent(i).key == SDLK_t) {
						selected = m_shader;
					}
					if (listener.getEvent(i).key == SDLK_y) {
						selected = m_vshader;
					}
				}
		  }

	int score = 10000;
	int lives = 3;

  // Update everything
  world->Update(dt);
  m_camera->Update(dt);
  updateFPS(dt);
  ui.Update(dt);
  //ImGui::Text("Score: %d\nLives: %d\nFPS: %.2f", score, lives, fps);


}

void Graphics::updateFPS(unsigned int dt)
{
	frameCount++;
	delay += dt;
	if (delay >= 1000)
	{
		fps = frameCount;
		delay = 0;
		frameCount = 0;
	}
}

void Graphics::Render()
{
  //set renderTarget
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LESS);
  glCullFace(GL_BACK);
  glViewport(0,0, width, height);

  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render all objects
  //TODO set flags
  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  TreeRender(world);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  //render frameBuffer to defaultBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glViewport(0,0, width, height);

  glClearColor(1.0, 0.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //render to default buffer
  addRenderTarget(m_screenShader, fbTex);

  //render UI
  ui.Render();

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::addRenderTarget(Shader *shader, GLuint texTarget)
{
	  shader->Enable();
	  glBindBuffer(GL_ARRAY_BUFFER, screen);
	  GLint posAttrib = glGetAttribLocation(shader->getShader(), "position");
	  GLint colAttrib = glGetAttribLocation(shader->getShader(), "texcoord");

	  //draw quad onto the screen
	  glEnableVertexAttribArray(posAttrib);
	  glEnableVertexAttribArray(colAttrib);
	  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), 0);
	  glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), (void*)(2*sizeof(float)));

	  //pass in output texture
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, texTarget);
	  glDrawArrays(GL_TRIANGLES, 0 ,6);
	  glDisableVertexAttribArray(posAttrib);
	  glDisableVertexAttribArray(colAttrib);
}

void Graphics::RenderList(vector<Object*> list)
{
	  for (int i = 0; i < list.size(); i++)
	  {
		  renderTarget = list[i];
		  TreeRender(renderTarget);
	  }
}

void Graphics::TreeRender(Object* object)
{
	  //enable correct shader
	  selected->Enable();

	  // Send in the projection and view to the shader
	  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	  //render this model
	  renderTarget = object;
	  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(object->GetModel()));
	  object->Render();

	  //render children if there are children
	  if (object->getChildren().size() != 0)
		  RenderList(object->getChildren());
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

