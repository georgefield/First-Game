#include "Sprite.h"


Sprite::Sprite(){

	_vboID = 0; //0 = not set to a buffer
}


Sprite::~Sprite(){

	if (_vboID != 0) { //if buffer created
		glDeleteBuffers(1, &_vboID); //delete the buffer indexed by _vboID from VRAM 
	}
}


void Sprite::init(float x, float y, float width, float height) {

	_x = x;
	_y = y;
	_width = width;
	_height = height;

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID); //create a vertex buffer ID
	}

	float vertexData[12]; //create an array of all vertexes

	////Set all vertices of square in array
	//triangle 1
	vertexData[0] = _x + _width;
	vertexData[1] = _y + _height;

	vertexData[2] = _x;
	vertexData[3] = _y + _height;

	vertexData[4] = _x;
	vertexData[5] = _y;

	//triangle 2
	vertexData[6] = _x;
	vertexData[7] = _y;

	vertexData[8] = _x + _width;
	vertexData[9] = _y;

	vertexData[10] = _x + _width;;
	vertexData[11] = _y + _height;
	

	glBindBuffer(GL_ARRAY_BUFFER, _vboID); //binds _vboID to a premade global GL_ARRAY_BUFFER which can be accessed by openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); //upload square vertexes to GPU and attach _vboID index to buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
}


void Sprite::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, _vboID); //binds _vboID to a GL_ARRAY_BUFFER
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); //vertex attrib array num, values per vertex (x & y), type of data of vertex (float), normalized?, ?, interleaved vertex data?

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
}