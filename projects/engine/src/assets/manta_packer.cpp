#include "manta_packer.hpp"
#include "model.hpp"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>


#include <console/console.hpp>

// Creates an .mmdl
//
// .mmdl's are structured as so
//
// const char[256] name;
// int noOfVerts;
// ... Vertices ...
// int noOfIndices;
// ... Indices ...
//
//
// Vertex:
// 	float[8] data;
// 		3 = position
// 		3 = normal
// 		2 = uv
//
// Indice aka int

void MantaPackModel(Model *model, std::string output) {
    if (!model) {
        printf("Provided model was a nullptr!\n");
        return;
    }

    std::ofstream file(output.c_str(), std::ofstream::out | std::ofstream::trunc);

    if (!file.is_open()) {
        printf("Failed to open / create file, %s\n", output.c_str());
        return;
    }

    //file << "# .mmdls's are meant to be used by the Manta engine\n";

    char name[256];
    strcpy(name, model->name.c_str());
    file.write(name, sizeof(name));

    int size = model->vertices.size();
    file.write(reinterpret_cast<char *>(&size), sizeof(int));

    printf("Compressing model to Manta-friendly format\n");

    for (int v = 0; v < model->vertices.size(); v++) {
        printf("\rWriting vertex data %c...");

        float data[8];
        Model::Vertex vert = model->vertices[v];

        data[0] = vert.position.x;
        data[1] = vert.position.y;
        data[2] = vert.position.z;

        data[3] = vert.normal.x;
        data[4] = vert.normal.y;
        data[5] = vert.normal.z;

        data[6] = vert.uv0.x;
        data[7] = vert.uv0.y;

        char buffer[sizeof(float) * 8];
        memcpy(buffer, data, sizeof(float) * 8);

        file.write(reinterpret_cast<char *>(&data), sizeof(float) * 8);
    }

    size = model->triangles.size();
    file.write(reinterpret_cast<char *>(&size), sizeof(int));

    printf("\n");

    for (int t = 0; t < model->triangles.size(); t++) {
        printf("\rWriting int data %c...");

        int data = model->triangles[t];
        file.write(reinterpret_cast<char *>(&data), sizeof(int));
    }

    printf("\n");

    file.close();
}
