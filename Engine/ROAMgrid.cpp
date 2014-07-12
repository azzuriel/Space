#include "ROAMgrid.h"

#include "GameMath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassicNoise.h"

#define tempres 1024

TerrainPatch::TerrainPatch(int offset_x, int offset_y)
    : m_map(nullptr)
    , m_worldX(offset_x)
    , m_worldY(offset_y)
    , m_leftVariance(nullptr)
    , m_rightVariance(nullptr)
    , m_varianceSize(0)
    , m_leftRoot(nullptr)
    , m_rightRoot(nullptr)
    , m_leftLeaves(0)
    , m_rightLeaves(0)
    , m_triPool(0)
    , m_poolSize(200000)
    , m_poolNext(0)
{
    //m_map = Heightmap_read(fn);
    m_map = new Heightmap();
    m_map->height = tempres;
    m_map->width = tempres;
    m_map->map = new float[tempres*tempres];
    auto map = m_map->map;
    auto a = rand()%1000/100.0f;
    for (int i =0; i<tempres;i++)
    {
        for (int j =0; j<tempres;j++)
        {
            float t = simplexnoise(offset_x + i/64.0F, offset_y + j/64.0F) + simplexnoise(offset_x + i/32.0F, offset_y + j/32.0F)/2.0F + simplexnoise(offset_x + i/16.0F, offset_y + j/16.0F)/4.0F;
            if(t < 0) {
                t = -t;
                t /= 30.0F;
            }
            if( t < 0.1) {
                t+= 0.1;
            }
            if(t > .6) {
                t /= 10.0F;
                t += .6;
            } 


            *map = t;
            m_map->maxZ = glm::max(t, m_map->maxZ);
            m_map->minZ = glm::min(t, m_map->minZ);
            ++map;
        }
    }

    if (m_map == nullptr) {
        return;
    }

    Heightmap_normalize(m_map);
    Heightmap_calculate_normals(m_map);

    m_triPool = new BTTNode[m_poolSize];
    //memset(m_triPool, 0, sizeof(BTTNode)*m_poolSize);

    m_leftRoot = allocateNode();
    m_rightRoot = allocateNode();
    m = new Mesh();
}

TerrainPatch::~TerrainPatch()
{
    delete m;
    delete [] m_triPool;
    delete [] m_leftVariance;
    delete [] m_rightVariance;
    if(m_map) {
        maps_delete(m_map);
    }
}


void TerrainPatch::computeVariance(int maxTessellationLevels)
{
    m_varianceSize = 2<<maxTessellationLevels;

    m_leftVariance = new float[m_varianceSize];
    m_rightVariance = new float[m_varianceSize];
    //memset(m_leftVariance, 0, sizeof(float)*m_varianceSize);
    //memset(m_rightVariance, 0, sizeof(float)*m_varianceSize);

    computeVarianceRecursive(
        maxTessellationLevels, 0, m_leftVariance, 1, m_map,
        0, m_map->height-1, Heightmap_get(m_map, 0, m_map->height-1),
        m_map->width-1, 0, Heightmap_get(m_map, m_map->width-1, 0),
        0, 0, Heightmap_get(m_map, 0, 0));
    computeVarianceRecursive(
        maxTessellationLevels, 0, m_rightVariance, 1, m_map,
        m_map->width-1, 0, Heightmap_get(m_map, m_map->width-1, 0),
        0, m_map->height-1, Heightmap_get(m_map, 0, m_map->height-1),
        m_map->width-1, m_map->height-1, Heightmap_get(m_map, m_map->width-1, m_map->height-1));
}

void TerrainPatch::reset()
{
    m_leftRoot->left_child = m_leftRoot->right_child = nullptr;
    m_rightRoot->left_child = m_rightRoot->right_child = nullptr;

    m_leftRoot->right_neighbor = m_leftRoot->left_neighbor = nullptr;
    m_rightRoot->right_neighbor = m_rightRoot->left_neighbor = nullptr;

    m_leftRoot->base_neighbor = m_rightRoot;
    m_rightRoot->base_neighbor = m_leftRoot;

    m_poolNext = 2;
}

void TerrainPatch::tessellate(const glm::vec3 &view, float errorMargin)
{
    tessellateRecursive(
        m_leftRoot, view, errorMargin,
        0, m_map->height-1,
        m_map->width-1, 0,
        0, 0,
        m_leftVariance, 1, m_map);
    tessellateRecursive(
        m_rightRoot, view, errorMargin,
        m_map->width-1, 0,
        0, m_map->height-1,
        m_map->width-1, m_map->height-1,
        m_rightVariance, 1, m_map);

    m_leftLeaves = BTTNode_number_of_leaves(m_leftRoot);
    m_rightLeaves = BTTNode_number_of_leaves(m_rightRoot);
}

void TerrainPatch::getTessellation(float *vertices, float *colors, float *normalTexels)
{
    int idx = 0;
    getTessellationRecursive(
        m_leftRoot, m_map, vertices, colors, normalTexels, &idx,
        0, m_map->height-1,
        m_map->width-1, 0,
        0, 0);
    getTessellationRecursive(
        m_rightRoot, m_map, vertices, colors, normalTexels, &idx,
        m_map->width-1, 0,
        0, m_map->height-1,
        m_map->width-1, m_map->height-1);
}

BTTNode *TerrainPatch::allocateNode()
{
    BTTNode *tri;

    if (m_poolNext >= m_poolSize) {
        return nullptr;
    }

    tri = &m_triPool[m_poolNext++];
    tri->left_child = tri->right_child = nullptr;

    return tri;
}

void TerrainPatch::split(BTTNode *node)
{
    if (node->left_child)
        return;

    if (node->base_neighbor && node->base_neighbor->base_neighbor != node) {
        split(node->base_neighbor);
    }

    node->left_child = allocateNode();
    node->right_child = allocateNode();

    if (!node->left_child || !node->right_child) {
        return;
    }

    node->left_child->base_neighbor = node->left_neighbor;
    node->left_child->left_neighbor = node->right_child;

    node->right_child->base_neighbor = node->right_neighbor;
    node->right_child->right_neighbor = node->left_child;

    // link left neighbor to the new children
    if (node->left_neighbor) {
        if (node->left_neighbor->base_neighbor == node)
            node->left_neighbor->base_neighbor = node->left_child;
        else if (node->left_neighbor->left_neighbor == node)
            node->left_neighbor->left_neighbor = node->left_child;
        else if (node->left_neighbor->right_neighbor == node)
            node->left_neighbor->right_neighbor = node->left_child;
    }

    // link right neighbor to the new children
    if (node->right_neighbor) {
        if (node->right_neighbor->base_neighbor == node)
            node->right_neighbor->base_neighbor = node->right_child;
        else if (node->right_neighbor->right_neighbor == node)
            node->right_neighbor->right_neighbor = node->right_child;
        else if (node->right_neighbor->left_neighbor == node)
            node->right_neighbor->left_neighbor = node->right_child;
    }

    // link base neighbor to the new children
    if (node->base_neighbor) {
        if (node->base_neighbor->left_child) {
            node->base_neighbor->left_child->right_neighbor = node->right_child;
            node->base_neighbor->right_child->left_neighbor = node->left_child;
            node->left_child->right_neighbor = node->base_neighbor->right_child;
            node->right_child->left_neighbor = node->base_neighbor->left_child;
        } else {
            split(node->base_neighbor);
        }
    } else {
        // edge triangle
        node->left_child->right_neighbor = nullptr;
        node->right_child->left_neighbor = nullptr;
    }
}

void TerrainPatch::tessellateRecursive(
    BTTNode *node, const glm::vec3 &view, float errorMargin,
    int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y,
    float *variance_tree, int variance_idx, Heightmap *map)
{
    if (variance_idx < m_varianceSize) {
        float center_x = (left_x + right_x) * 0.5f;
        float center_y = (left_y + right_y) * 0.5f;

        auto point = normalize(glm::vec3(center_x/m_map->width-0.5, center_y/m_map->height-0.5, -0.5));
        float distance = 1 + glm::distance(point, view);
        float variance = variance_tree[variance_idx]/distance;
        
        //if(view.z > 1) {
        //	variance /= view.z;
        //}

        if (variance > errorMargin || variance_idx < 32) {
            split(node);
            if (node->left_child && ((abs(left_x - right_x) >= 3) || (abs(left_y - right_y) >= 3)))
            {
                tessellateRecursive(
                    node->left_child, view, errorMargin*distance,
                    apex_x, apex_y, left_x, left_y, center_x, center_y,
                    variance_tree, (variance_idx<<1), map);
                tessellateRecursive(
                    node->right_child, view, errorMargin*distance,
                    right_x, right_y, apex_x, apex_y, center_x, center_y,
                    variance_tree, (variance_idx<<1)+1, map);
            }
        }
    }
}

void TerrainPatch::computeVarianceRecursive(
    int maxTessellationLevels, int level, float *varianceTree, int idx, Heightmap *map,
    int left_x, int left_y, float left_z,
    int right_x, int right_y, float right_z,
    int apex_x, int apex_y, float apex_z)
{
    int center_x = (left_x + right_x) / 2;
    int center_y = (left_y + right_y) / 2;
    float center_z = Heightmap_get(map, center_x, center_y);

    if (level < maxTessellationLevels) {
        computeVarianceRecursive(
            maxTessellationLevels, level+1, varianceTree, (idx<<1), map,
            apex_x, apex_y, apex_z,
            left_x, left_y, left_z,
            center_x, center_y, center_z);
        computeVarianceRecursive(
            maxTessellationLevels, level+1, varianceTree, (idx<<1)+1, map,
            right_x, right_y, right_z,
            apex_x, apex_y, apex_z,
            center_x, center_y, center_z);

        varianceTree[idx] = MAX(varianceTree[(idx<<1)],
            varianceTree[(idx<<1)+1]);
    } else {
        varianceTree[idx] = fabs(center_z - ((left_z + right_z)*0.5));
    }
}

void TerrainPatch::getTessellationRecursive(
    BTTNode *node, Heightmap *map,
    float *vertices, float *colors, float *normalTexels, int *idx,
    int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y)
{
    if (node->left_child) {
        int center_x = (left_x + right_x) / 2;
        int center_y = (left_y + right_y) / 2;

        getTessellationRecursive(
            node->left_child, map, vertices, colors, normalTexels, idx,
            apex_x, apex_y, left_x, left_y, center_x, center_y);
        getTessellationRecursive(
            node->right_child, map, vertices, colors, normalTexels, idx,
            right_x, right_y, apex_x, apex_y, center_x, center_y);
    } else {
        // we're at leaf
        vertices[*idx+0] =  left_x /(float) (map->width - 1);
        vertices[*idx+1] =  left_y /(float) (map->height - 1);
        vertices[*idx+2] = Heightmap_get(map, left_x, left_y);
        vertices[*idx+3] =  right_x /(float) (map->width - 1);
        vertices[*idx+4] =  right_y /(float) (map->height - 1);
        vertices[*idx+5] = Heightmap_get(map, right_x, right_y);
        vertices[*idx+6] =  apex_x /(float) (map->width - 1);
        vertices[*idx+7] =  apex_y /(float) (map->height - 1);
         vertices[*idx+8] = Heightmap_get(map, apex_x, apex_y);

        colors[*idx+0] = 1;
        colors[*idx+1] = 1;
        colors[*idx+2] = 1;
        colors[*idx+3] = 1;
        colors[*idx+4] = 1;
        colors[*idx+5] = 1;
        colors[*idx+6] = 1;
        colors[*idx+7] = 1;
        colors[*idx+8] = 1;

        normalTexels[(*idx/9)*6+0] =  left_x  /(float) (map->width - 1);
        normalTexels[(*idx/9)*6+1] =  left_y  /(float) (map->height - 1);
        normalTexels[(*idx/9)*6+2] =  right_x /(float) (map->width - 1);
        normalTexels[(*idx/9)*6+3] =  right_y /(float) (map->height - 1);
        normalTexels[(*idx/9)*6+4] =  apex_x  /(float) (map->width - 1);
        normalTexels[(*idx/9)*6+5] =  apex_y  /(float) (map->height - 1);

        *idx += 9;
    }
}

void TerrainPatch::Bind(float *vertices, float *colors, float *normalTexels)
{
    auto leaves = m_leftLeaves + m_rightLeaves;
    m->Verteces.resize(leaves*3);
    m->Indeces.resize(leaves*3);

    for (int i=0; i<leaves*3; i++)
    {
        m->Verteces[i].Position = normalize(glm::vec3(vertices[i*3]-0.5, vertices[i*3+1]-0.5, -0.5)) * ((97+vertices[i*3+2]*3)/100.0f);
        m->Verteces[i].Normal = m->Verteces[i].Position;
        m->Indeces[i] = i;
        m->Verteces[i].Uv = glm::vec2(normalTexels[i*2], normalTexels[i*2+1]);
    }
    m->Bind(1);
}

void TerrainPatch::Render()
{
    m->Render();
}

void TerrainPatch::FreeMaps()
{
    maps_delete(m_map);
}
