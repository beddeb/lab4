#pragma once

#include "../../include/graph.hpp"


template <typename T>
T getValue(int index);

template <typename T>
void getValues(T& v1, T& v2, T& v3);

void testAddVertex();

void testAddEdge();

void testAddDuplicateEdge();

void testRemoveEdge();

void testRemoveVertex();

void testGetNeighbors();

void testGetEdges();

void testGetVertexCount();

void testGetEdgeCount();

void testGraph();