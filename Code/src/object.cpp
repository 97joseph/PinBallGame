#include "object.h"
//
string Object::rootDir = "assets/";
vector<ModelInstance> Object::modelBank;
vector<TexInstance> Object::textureBank;

Object::Object()
{  
	multiplier = 1.0f;
	size = 1.0f;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));

	//load default textures
	loadTexture("textures/ERROR_TEXTURE.jpg");
	loadNormal("textures/ERROR_TEXTURE.jpg");
	loadModel("models/cube.obj");
}

Object::~Object()
{
  for (int i = 0; i < children.size(); i++)
  {
	  delete children[i];
  }

  children.clear();
}

void Object::init()
{
	//initialize root directory
	Object test;
	if (!test.loadModel("models/cube.obj"))
	{
		rootDir = "../assets/";
		cout << "ROOT CHANGED" << endl;
	}
}

void Object::end()
{
	//clear all textures and models
	for (int i = 0; i < modelBank.size(); i++)
	{
		glDeleteBuffers(1, &modelBank[i].VB);
		glDeleteBuffers(1, &modelBank[i].IB);
	}

	for (int i = 0; i < textureBank.size(); i++)
	{
		glDeleteTextures(1, &textureBank[i].texture);
	}

	modelBank.clear();
	textureBank.clear();
}

void Object::Begin()
{
	//start children
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Begin();
	}
}

void Object::setVisual(string model, string albedo, string normal)
{
	loadModel(model);
	loadTexture(albedo);
	loadNormal(normal);
}

bool Object::loadModel(string filename)
{
	  ModelInstance model = pushModel(filename);
	  if (model.VB != 0)
	  {
		  modelData = model;
		  return true;
	  }
	  else
		  return false;
}

bool Object::loadTexture(string filename)
{
	textureData = pushTexture(filename, GL_TEXTURE0);
	if (textureData.texture == 0)
		return false;
	else
		return true;
}

void Object::loadTexture(string filename, int index)
{
	texPointer.push_back(pushTexture(filename, GL_TEXTURE0 + index).texture);
	texIndex.push_back(index);
}

void Object::loadNormal(string filename)
{
	normal = pushTexture(filename, GL_TEXTURE1);
}

TexInstance Object::pushTexture(string filename,GLenum position)
{
	//search if texture already exists
	for (int i  = 0; i < textureBank.size(); i++)
	{
		if (textureBank[i].name.compare(filename) == 0)
		{
			//if exists, return pointer
			return textureBank[i];
		}
	}

	//texture doesn't exist yet, add in
	  loader fileLoader;
	  Texture texture;

	  if (fileLoader.loadTexture(rootDir + filename, texture))
	  {
		  GLuint final;
		  //texture loaded
		  glGenTextures(1, &final);
		  bindTex(final, position);
		  setTex(texture);
		  textureBank.push_back(TexInstance(final, filename));
		  return TexInstance(final, filename);
	  }

	  //error, return 0
	  return TexInstance(0, "NULL");
}

ModelInstance Object::pushModel(string filename)
{
	//search for model
	for (int i = 0; i < modelBank.size(); i++)
	{
		if (modelBank[i].name.compare(filename) == 0)
		{
			//if exists, return pointer
			return modelBank[i];
		}
	}

	//model doesn't exist yet, add
	  loader fileLoader;
	  obj object;
	  ModelInstance final;
	  if (fileLoader.loadObject(rootDir + filename, object))
	  {
		  GLuint tempVB;
		  GLuint tempIB;

		  //model loading
		  glGenBuffers(1, &tempVB);
		  glBindBuffer(GL_ARRAY_BUFFER, tempVB);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (object.getVerts().size()+1), &object.getVerts()[0], GL_STATIC_DRAW);

		  glGenBuffers(1, &tempIB);
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempIB);
		  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object.getIndices().size(), &object.getIndices()[0], GL_STATIC_DRAW);

		  //add to model bank
		  final = ModelInstance(tempVB, tempIB, object.getIndices().size(), filename);
		  modelBank.push_back(final);

		  //return
		  return final;
	  }

	//error, return empty vector
	return final;
}

GLuint Object::bindTex(GLuint &bind, GLenum unit)
{
	  glActiveTexture(unit);
	  glBindTexture(GL_TEXTURE_2D, bind);
}

void Object::setTex(Texture texture)
{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.column, texture.row, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Object::Update(unsigned int dt)
{
	  //render
	  model = mtranslate * mscale * mrotate;
	  //update keyboard
	  for (int i = 0; i < listener.getSize(); i++)
	  {
		  keyboard(listener.getEvent(i));
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void Object::setMultiplier(float mult)
{
	multiplier = mult;
}

void Object::keyboard(eventType event)
{
	//does nothing, is virtual for inherited objects
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glBindBuffer(GL_ARRAY_BUFFER, modelData.VB);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

  bindTex(textureData.texture, GL_TEXTURE0);
  bindTex(normal.texture, GL_TEXTURE1);

  for (int i = 0; i < texPointer.size(); i++)
  {
	  bindTex(texPointer[i], GL_TEXTURE0 + texIndex[i]);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.IB);
  glDrawElements(GL_TRIANGLES, modelData.size, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

std::vector<Object*> Object::getChildren()
{
	return children;
}
void Object::addChild(Object *child)
{
	children.push_back(child);
}

float Object::getSize()
{
	return size;
}

void Object::translate(glm::vec3 translation)
{
	mtranslate = glm::translate(translation);
	//reset model matrix
	model = mtranslate * mscale * mrotate;
}

void Object::rotate(float angle, glm::vec3 axis)
{
	mrotate = glm::rotate(angle, axis);
	//reset model matrix
	model = mtranslate * mscale * mrotate;
}

void Object::scale(float amount)
{
	size  = amount;
	mscale = glm::scale(glm::vec3(size));
	//reset model matrix
	model = mtranslate * mscale * mrotate;
}
