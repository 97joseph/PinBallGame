
#include <obj.h>

obj::obj()
{
}

obj::~obj()
{
	vertRaw.empty();
	vertices.empty();
	indices.empty();
}

void obj::addVert(Vertex vert)
{
	vertices.push_back(vert);
}

void obj::addVert(Vertex vert, int index)
{
	vector<Vertex>::iterator i;

	//insert if it doesn't yet exist
	i = vertices.begin()+index;
	if (vertices.at(index).vertex != vert.vertex)
	{
		vertices.erase(i);
		vertices.insert(i, vert);
	}
	else
	{
		//otherwise, average normals
		Vertex temp = vertices.at(index);
		temp.color = glm::normalize(temp.color + vert.color);
		vertices.erase(i);
		vertices.insert(i, temp);
	}
}

void obj::addIndice(unsigned int ind)
{
	//add indice and auto fix indexing
	indices.push_back(ind-1);
}

void obj::addRaw(glm::vec3 raw)
{
	vertRaw.push_back(raw);

	//allocate size for new vertex in vertices
	glm::vec3 item;
	glm::vec2 item2;
	Vertex temp(item,item, item2);
	vertices.push_back(temp);
}

vector<Vertex> obj::getVerts()
{
	return vertices;
}

vector<unsigned int> obj::getIndices()
{
	return indices;
}

vector<glm::vec3> obj::getRawVerts() {
	return vertRaw;
}
