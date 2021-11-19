#pragma once

#include <iostream>

#include "../Classes/Math.h"
#include "../Types/AllTypes.h"

using namespace GraphTypes;

class Connection;       //basic element of a graph
class DirectConnection; //basic element of direct graph
class Graph;            //https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)
class PhysicConnection; //visual representation of a graph edge
class Node;             //visual representation of a graph node

class Connection
{
protected:
    point_t p1; //first node
    point_t p2; //second node
    
public:
    Connection();
    
    Connection(const Connection& connection);
    
    Connection(point_t p1, point_t p2);
    
    //The function changes value of the first node.
    void ChangePoint1(point_t value);
    
    //The function changes value of the second node.
    void ChangePoint2(point_t value);
    
    //The function returns value of the first node.
    point_t GetPoint1();
    
    //The function returns value of the second node.
    point_t GetPoint2();
    
    //The function returns maximun value of nodes. 
    point_t GetMaxPoint();
    
    //The function returns minimal value of nodes.
    point_t GetMinPoint();
    
    //The function checks the connection for indicated node.
    bool IsCollectPoint(point_t p);
    
    //The function return new connection this swaped nodes.
    Connection Swap();
    
    //The function swaps nodes in the connection.
    void SwapThis();
    
    void operator=(Connection connection);
    
    //Values of nodes in connections are same.
    bool operator==(Connection connection);
    
    //Values of nodes in connections are different.
    bool operator!=(Connection connection);
    
    /*
    If the value of the first node of the this connection less then the value of the first node of the indicated connection, the function return true.
    Else if the value of the second node of the this connection less then the value of the second node of the indicated connection, the function return true.
    Else the function return false.
    */
    bool operator<(Connection connection);
    
    /*
    If the value of the first node of the this connection larger then the value of the first node of the indicated connection, the function return true.
    Else if the value of the second node of the this connection larger then the value of the second node of the indicated connection, the function return true.
    Else the function return false.
    */
    bool operator>(Connection connection);
    
    ~Connection();
};

class DirectConnection : public Connection
{
public:
    DirectConnection();
    DirectConnection(const DirectConnection& connection);
    DirectConnection(point_t p1, point_t p2);
    
    void Set(DirectConnection* connection);
    DirectConnection Swap();
    
    void operator=(DirectConnection connection);
    
    /*
    Value of the first node of the this connection is same this value of the first node of the input connection, and value of the second node of the this connection is same this value of the second node of the input connection.
    */
    bool operator==(DirectConnection connection);
    
    //The function return oposite value for "operator==".
    bool operator!=(DirectConnection connection);
    
    ~DirectConnection();
};

std::ostream& operator<<(std::ostream& stream, Connection connection);



class Graph
{
private:
    //"Next connections" is connections been near the connection with indicated "connection_index".
    
    //The function return array of connections that the connection with indicated "connection_index". 
    Connection* GetNextConnectionsArray(point_t connection_index);
    
    //The function return count of elements of array of connections that the connection with indicated "connection_index".
    point_t GetNextConnectionsCount(point_t connection_index);
    
    //The function check loops in the graph considering that the graph is connected.
    bool IsHaveCycleAsConnected();
protected:
    point_t connections_count;  //count of edges in the graph
    Connection* connections;    //array of edges of the graph
public:
    Graph();
    Graph(const Graph& graph);
    Graph(Connection* connections, point_t connections_count);
    Graph(MatNI matrix);
    Graph(MatNI* matrix);
    
    //The function get connected edges and write they to "return_graph" after that return true.
    //If the graph is connected the function return false.
    bool Divide(Graph* return_graph);
    
    //The function return count of all adges.
    point_t GetConnectionsCount();
    
    //The function return count of indicated adges.
    point_t GetConnectionsCount(Connection connection);
    
    //The function return count of indicated adges.
    point_t GetConnectionsCount(Connection* connection);
    
    //The function return count of adges that collect indicated node.
    point_t GetConnectionsCount(point_t p);
    
    //The function return maximun value of all nodes.
    point_t GetMaxPoint();
    
    //The function return nodes array.
    point_t* GetPointsArray();
    
    //The function return nodes count.
    point_t GetPointsCount();
    
    //The function create a new block of memory and return pointer to it.
    //The function can return nullptr.
    //Don't foget clear this block after use.
    Connection* GetConnectionsArray();
    
    //The function create a new block of memory and return pointer to it.
    //The function return connections that have indicated point.
    //The function can return nullptr.
    //Don't foget clear this block after use.
    Connection* GetConnectionsArray(point_t point_number);
    
    //The function chec existing point with indicated number in the graph.
    bool IsCollectPoint(point_t point_number);
    
    //Properties
    
    //The function chec that graph is basic.
    bool IsBasic();
    
    //The function chec that graph is connected.
    bool IsConnected();
    
    //The function chec that graph has cycle.
    bool IsHaveCycle();
    
    //The function chec that graph is thee.
    bool IsTree();
    
    //Properties
    
    //The function sorts connections in the praph.
    void Sort();
    
    //only sorted
    
    //The function return hight of view representation of the graph.
    unsigned GetHight();
    
    //The function return width of view representation of the graph.
    unsigned GetWidth();
    
    //only sorted
    
    void operator=(Graph graph);
    
    //All nodes are same.
    bool operator==(Graph graph);
    
    bool operator!=(Graph graph);
    
    ~Graph();
};

std::ostream& operator<<(std::ostream& stream, Graph graph);

class Node
{
public:
    
    
    float radius;   //radius of the point
    Vec2F position; //position of the point
    float text_size;//text size inside the point
    point_t number; //number of the point
    
    Node(const Node& point);
    
    Node(
        point_t number = POINT_DEFAULT_NUMBER,  //set number of the point
        Vec2F position = POINT_DEFAULT_POSITION,//set position of the point
        float radius = POINT_DEFAULT_RADIUS,    //set radius of the point
        float text_size = POINT_TEXT_SIZE_AUTO);//set text size of the point
    
    //The function returns poiter to string this number of the point.
    char* GetNumberAsTextC();
    
    //The function returns poiter to string (as integer array) this number of the point.
    int* GetNumberAsTextI();
    
    //The function returns count of signs in the value of the point.
    unsigned GetNumberTextLength();
    
    //The function updates size of text in the point.
    void Update();
    
    void operator=(Node point);
    
    ~Node();
};

class PhysicConnection
{
protected:
    Vec2F p1; //point of the center segment
    Vec2F p2; //point of the center segment
    
    //p1_p -> p1 -> p2 -> p2_p
    
public:
    Vec2F* p1_p;//pointer to pisition of point
    Vec2F* p2_p;//pointer to pisition of point
    float shift_x;//shift to normal direction
    float shift_y;//shift to perpendicular direction
    
    PhysicConnection(const PhysicConnection& connection);
    
    PhysicConnection(
        Node* p1,  //set pointer to first point position
        Node* p2,  //set pointer to second point position
        float shift_x = CONNECTION_DEFAULT_DISTANCE_TO_POINT,   //set shift to normal direction
        float shift_y = CONNECTION_DEFAULT_SHIFT);              //set shift to perpendicular direction
    
    //The function returns segment between start and end points.
    Segment GetSegment();
    
    //The function returns indicated segment.
    Segment GetSegment(ConnectionTypes::segment_id_t segment_id);
    
    //The function updates positions of points.
    void Update();
    
    void operator=(PhysicConnection connection);
    
    ~PhysicConnection();
};








