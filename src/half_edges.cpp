#include <half_edges.h>

Mesh::Mesh(GLWidget277 *context) : Drawable(context) {}

void Mesh::createMeshVertexPositions(std::vector<glm::vec4> &mesh_vert_pos)
{
    for(int i = 0; i < idList.size(); i++)
    {
         mesh_vert_pos.push_back(glm::vec4(idList[i]->pos,1));
    }
}

void Mesh::createMeshVertexNormals(std::vector<glm::vec4> &mesh_vert_nor)
{
    int count = 0;
    for(Face* f : faces)
    {
        HalfEdge *half = f->start;
        int temp = 0;

        while(1)
        {
            glm::vec3 v0 = half->vert->pos;
            glm::vec3 v1 = half->next->vert->pos;
            glm::vec3 v2 = half->next->next->vert->pos;
            glm::vec4 n = glm::vec4(glm::vec3(-1) * glm::normalize(glm::cross(v1-v0, v1-v2)), 1);
            mesh_vert_nor.push_back(n);
            temp++;
             half = half->next;
            if(half == f->start)
            {
                count += temp; break;
            }
        }
    }
}

void Mesh::createMeshVertexColors(std::vector<glm::vec4> &mesh_vert_col)
{
    for(int i = 0; i < color_List.size(); i++)
    {
        mesh_vert_col.push_back(color_List[i]);
    }
}

void Mesh::createMeshIndices(std::vector<GLuint> &mesh_idx)
{
    int count = 0;
    for(Face* f : faces)
    {
        std::vector<int> tris;
        HalfEdge *half = f->start;
        int tmp = 0;

        while(1)
        {
            idList[tmp + count] = half->vert;
            color_List[tmp + count] = f->color;
            tris.push_back(tmp++);
            if(tris.size() == 3)
            {
                mesh_idx.push_back(tris[0] + count);
                mesh_idx.push_back(tris[1] + count);
                mesh_idx.push_back(tris[2] + count);
                tris.erase(tris.begin() + 1, tris.begin() + 2);
            }
             half = half->next;
            if(half == f->start)
            {
                count += tmp; break;
            }
        }
    }

}

void Mesh::create()
{
    std::vector<GLuint> mesh_idx;
    std::vector<glm::vec4> mesh_vert_pos;
    std::vector<glm::vec4> mesh_vert_nor;
    std::vector<glm::vec4> mesh_vert_col;

    createMeshIndices(mesh_idx);
    createMeshVertexPositions(mesh_vert_pos);
    createMeshVertexNormals(mesh_vert_nor);
    createMeshVertexColors(mesh_vert_col);

    count = mesh_idx.size();

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_idx.size() * sizeof(GLuint), mesh_idx.data(), GL_STATIC_DRAW);
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_vert_pos.size() * sizeof(glm::vec4), mesh_vert_pos.data(), GL_STATIC_DRAW);
    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_vert_nor.size() * sizeof(glm::vec4), mesh_vert_nor.data(), GL_STATIC_DRAW);
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_vert_col.size() * sizeof(glm::vec4), mesh_vert_col.data(), GL_STATIC_DRAW);

    std::cout<<"mesh ready!";
}

//////
//Connect HalfEdges
///
ConnectMesh::ConnectMesh(GLWidget277 *context) : Mesh(context) {}

void ConnectMesh::create()
{
    this->Mesh::create();
}

void ConnectMesh::connect(std::vector<glm::vec3> obj_vertexs,
                          std::vector<glm::vec3> obj_norms,
                          std::vector<glm::vec3> obj_uvs,
                          std::vector<int> obj_face_index,
                          std::map<int, int> obj_index_vertexs)
{
    // Init
    for(unsigned int i = 0; i < obj_vertexs.size(); i++)
    {
        vertices.push_back(new Vertex(i, obj_vertexs[i]));
    }

    for(unsigned int i = 0; i < obj_face_index.size(); i++)
    {
        faces.push_back(new Face(i, glm::vec4(1)));
    }

    for(unsigned int i = 0; i < obj_index_vertexs.size(); i++)
    {
        halfedges.push_back(new HalfEdge(i));
    }

    //Random Color
    srand(time(NULL));

    int count=0;

    for(unsigned int i = 0; i < faces.size(); i++)
    {
        float r =(rand() % 255 + 1.0) / 255.0;
        float g =(rand() % 255 + 1.0) / 255.0;
        float b =(rand() % 255 + 1.0) / 255.0;

        count +=obj_face_index[i];
        faces[i]->start = halfedges[count - 1];
        faces[i]->color = glm::vec4(r,g,b,1);
    }

    //Connect
    int s = 0, e = 0;

    for(unsigned int i = 0; i < faces.size(); i++)
    {
        s = e;
        e += obj_face_index[i];
        int temp_s = s;
        while(temp_s < e) {
            halfedges[temp_s]->face = faces[i];
            if(temp_s == e - 1) {
                halfedges[temp_s]->next = halfedges[s];
                unsigned int a = obj_index_vertexs[temp_s];
                unsigned int b = obj_index_vertexs[s];
                triangle[std::make_pair(a, b)] = halfedges[s];
            } else {
                halfedges[temp_s]->next = halfedges[temp_s+1];
                unsigned int a = obj_index_vertexs[temp_s];
                unsigned int b = obj_index_vertexs[temp_s+1];
                triangle[std::make_pair(a, b)] = halfedges[temp_s + 1];
            }
            vertices[obj_index_vertexs[temp_s]]->edge = halfedges[temp_s];
            halfedges[temp_s]->vert = vertices[obj_index_vertexs[temp_s]];
            temp_s++;
        }
    }

    //Get Symmetry Edges
    for(auto & edgeEntry : triangle)
    {
        HalfEdge * fe = edgeEntry.second;
        unsigned int a = edgeEntry.first.first;
        unsigned int b = edgeEntry.first.second;
        if(triangle.find(std::make_pair(b,a)) != triangle.end()) {
            fe->sym = triangle[std::make_pair(b,a)];
        }
    }

    triangle.clear();

    //Edge
    for(unsigned int i = 0; i < halfedges.size(); i++)
    {
        edgeList[i] = halfedges[i];
    }

    //Face
    for(unsigned int i = 0; i < faces.size(); i++)
    {
        faceList[i] = faces[i];
    }

    //Normals
    for(unsigned int i = 0; i <obj_index_vertexs.size(); i++)
    {
        norList[i] = glm::vec4(obj_norms[i],1);
    }

    std::cout<<"\nConnection Ready!\n";

}




