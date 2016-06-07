#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char* filepath)
{
	// Load data from obj file
	parse(filepath);

	for (int i = 0; i < vertices.size(); ++i)
	{
		Vertex newVertex;
		newVertex.position = vertices[i];
		newVertex.normal = normals[i];

		posNorm.push_back(newVertex);
	}

	this->toWorld = glm::mat4(1.0f);

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, posNorm.size() * sizeof(Vertex), &posNorm.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(int), &faces.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		6 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

OBJObject::~OBJObject()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char* filepath)
{
	FILE * fp; // File
	char currentChar, nextChar; // Character read from file
	float vx, vy, vz; // x,y,z values for a vector
	float r, g, b; // red, green, blue values

				   // Open file
	fp = fopen(filepath, "r");

	// There was an error opening file
	if (fp == NULL)
	{
		std::cerr << "Error loading file: " << filepath << std::endl;
		exit(-1);
	}

	// parse
	while ((currentChar = fgetc(fp)) != EOF)
	{
		// vector or vector normal
		if (currentChar == 'v')
		{
			nextChar = fgetc(fp);

			// vertices
			if (nextChar == ' ')
			{
				// Read in the vertex values
				fscanf(fp, "%f %f %f %f %f %f", &vx, &vy, &vz, &r, &g, &b);

				// Create the vector
				glm::vec3 vertexVector = glm::vec3(vx, vy, vz);

				// Store vector
				vertices.push_back(vertexVector);

				// Look at all vertices to find min and max
				if (vertices.size() == 1)
				{
					glm::vec3 initialVertex = vertices.at(0);

					// Max and min of x
					max[0] = min[0] = initialVertex.x;
					// Max and min of y
					max[1] = min[1] = initialVertex.y;
					// Max and min of z
					max[2] = min[2] = initialVertex.z;
				}
				// Set the max and mins
				else
				{
					if (vertexVector.x > max[0])
						max[0] = vertexVector.x;
					if (vertexVector.x < min[0])
						min[0] = vertexVector.x;

					if (vertexVector.y > max[1])
						max[1] = vertexVector.y;
					if (vertexVector.y < min[1])
						min[1] = vertexVector.y;

					if (vertexVector.z > max[2])
						max[2] = vertexVector.z;
					if (vertexVector.z < min[2])
						min[2] = vertexVector.z;
				}
			}
			// vertex normal
			else if (nextChar == 'n')
			{
				// Read in the vertex normal values
				fscanf(fp, " %f %f %f", &vx, &vy, &vz);

				// Create the vector
				glm::vec3 normalVector = glm::vec3(vx, vy, vz);

				// Store the normal vector
				normals.push_back(normalVector);
			}

		}
		// faces line
		else if (currentChar == 'f')
		{
			int index1 = 0, index2 = 0, index3 = 0, empty = 0;

			fscanf(fp, " %d//%d %d//%d %d//%d", &index1, &empty, &index2, &empty, &index3, &empty);

			// Change indices to 0 base
			index1 -= 1;
			index2 -= 1;
			index3 -= 1;

			// Save the indices
			faces.push_back(index1);
			faces.push_back(index2);
			faces.push_back(index3);
		}
		// comment line
		else if (currentChar == '#')
		{
			// Eat up characters until newline or EOF is reached
			while (currentChar != '\n' && currentChar != EOF)
			{
				currentChar = fgetc(fp);
			}

			// Done with parsing
			if (currentChar == EOF)
				break;
		}
	}

	// Close file
	fclose(fp);

	std::cerr << "Done Parsing" << std::endl;

	// Center obj
	// Find averages of x,y,z
	float avgX = (max[0] + min[0]) / 2.0f;
	float avgY = (max[1] + min[1]) / 2.0f;
	float avgZ = (max[2] + min[2]) / 2.0f;

	// Create avg vector
	glm::vec3 avgVector = glm::vec3(avgX, avgY, avgZ);

	// Update all vertex vectors
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i] = vertices[i] - avgVector;
	}

	// Normalize objects
	// Change maxes to match center
	max[0] -= avgX;
	max[1] -= avgY;
	max[2] -= avgZ;
	// Find the longest dimensions
	float maxDimension = max[0];
	if (maxDimension < max[1])
	{
		maxDimension = max[1];
	}

	if (maxDimension < max[2])
	{
		maxDimension = max[2];
	}

	// Normalize all the vertices
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i] = vertices[i] / maxDimension;
	}

}

void OBJObject::draw()
{
	draw(glm::mat4(1.0f));
}

void OBJObject::draw(glm::mat4 transformMatrix)
{
	glm::mat4 model = transformMatrix * this->toWorld;
	glm::mat4 view = Window::V;
	glm::mat4 projection = Window::P;

	// Get camera position
	Camera* camera = Window::getCamera();
	glm::vec3 cameraPosition = camera->getPosition();

	glUseProgram(shaderProgram);

	GLuint modelID = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	GLuint viewID = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);

	GLuint projectionID = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);

	GLuint cameraID = glGetUniformLocation(shaderProgram, "cameraPosition");
	glUniform3f(cameraID, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)faces.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void OBJObject::update()
{
}


void OBJObject::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
}