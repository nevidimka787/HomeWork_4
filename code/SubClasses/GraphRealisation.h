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
class PhysicGraph;      //visual representation of a graph
class Point;

class Point
{
public:
    point_t number;
    color_t color;
    
    Point();
    
    void operator=(Point point);
    
    ~Point();
};

class Connection
{
protected:
    point_t p1; //first node
    point_t p2; //second node
    color_t c1; //first node color
    color_t c2; //second node color
    
public:
    Connection();
    
    Connection(const Connection& connection);
    
    Connection(point_t p1, point_t p2, color_t c1 = 0, color_t c2 = 0);
    
    //The function changes color of the first node.
    void ChangeColor1(point_t value);
    
    //The function changes color of the second node.
    void ChangeColor2(point_t value);
    
    //The function changes value of the first node.
    void ChangePoint1(point_t value);
    
    //The function changes value of the second node.
    void ChangePoint2(point_t value);
    
    //The function returns color of the first node.
    point_t GetColor1() const;
    
    //The function returns color of the second node.
    point_t GetColor2() const;
    
    //The function returns value of the first node.
    point_t GetPoint1() const;
    
    //The function returns value of the second node.
    point_t GetPoint2() const;
    
    //The function returns maximun value of nodes. 
    point_t GetMaxPoint() const;
    
    //The function returns minimal value of nodes.
    point_t GetMinPoint() const;
    
    //The function checks the connection for indicated node.
    bool IsHavePoint(point_t p) const;
    
    //If a color of first point and a color of second point are same then the funtion return true. .
    bool IsPointsHaveSameColor() const;
    
    //If this connection has one ore more same points with indicated connection then fte function return true.
    bool IsHaveSamePoint(const Connection* connection) const;
    
    //If the connection has a lone point the the function return true.
    bool IsConnectOnePoint() const;
    
    //The function return new connection this swaped nodes.
    Connection Swap() const;
    
    //The function swaps nodes in the connection.
    void SwapThis();
    
    void operator=(Connection connection);
    
    //Values of nodes in connections are same.
    bool operator==(Connection connection) const;
    
    //Values of nodes in connections are different.
    bool operator!=(Connection connection) const;
    
    /*
    If the value of the first node of the this connection less then the value of the first node of the indicated connection, the function return true.
    Else if the value of the second node of the this connection less then the value of the second node of the indicated connection, the function return true.
    Else the function return false.
    */
    bool operator<(Connection connection) const;
    
    /*
    If the value of the first node of the this connection larger then the value of the first node of the indicated connection, the function return true.
    Else if the value of the second node of the this connection larger then the value of the second node of the indicated connection, the function return true.
    Else the function return false.
    */
    bool operator>(Connection connection) const;
    
    ~Connection();
};

class DirectConnection : public Connection
{
public:
    DirectConnection();
    DirectConnection(const DirectConnection& connection);
    DirectConnection(point_t p1, point_t p2);
    
    void Set(DirectConnection* connection);
    DirectConnection Swap() const;
    
    void operator=(DirectConnection connection);
    
    /*
    Value of the first node of the this connection is same this value of the first node of the input connection, and value of the second node of the this connection is same this value of the second node of the input connection.
    */
    bool operator==(DirectConnection connection) const;
    
    //The function return oposite value for "operator==".
    bool operator!=(DirectConnection connection) const;
    
    ~DirectConnection();
};

std::ostream& operator<<(std::ostream& stream, Connection connection);



class Graph
{
private:
    //"Next connections" is connections been near the connection with indicated "connection_index".
    
    //The function return array of connections that the connection with indicated "connection_index". 
    Connection* GetNextConnectionsArray(point_t connection_index) const;
    
    //The function return count of elements of array of connections that the connection with indicated "connection_index".
    point_t GetNextConnectionsCount(point_t connection_index) const;
    
    //The function check loops in the graph considering that the graph is connected.
    bool IsHaveCycleAsConnected() const;
    
    //If the graph was painted correct then the function returns true.
    bool CheckPaint() const;
protected:
    point_t connections_count;  //count of edges in the graph
    Connection* connections;    //array of edges of the graph
public:
    Graph();
    Graph(const Graph& graph);
    Graph(Connection* connections, point_t connections_count);
    
    //The function get connected edges and write they to "return_graph" after that return true.
    //If the graph is connected the function return false.
    bool Divide(Graph* return_graph);
    
    //The function return count of all adges.
    point_t GetConnectionsCount() const;
    
    //The function return count of indicated adges.
    point_t GetConnectionsCount(Connection connection) const;
    
    //The function return count of indicated adges.
    point_t GetConnectionsCount(Connection* connection) const;
    
    //The function return count of adges that collect indicated node.
    point_t GetConnectionsCount(point_t p) const;
    
    //The function return maximun value of all nodes.
    point_t GetMaxPoint() const;
    
    //The function return nodes array.
    point_t* GetPointsArray() const;
    
    //The function return colored nodes array.
    Point* GetColoredPointsArray() const;
    
    //The function return nodes count.
    point_t GetPointsCount() const;
    
    //The function create a new block of memory and return pointer to it.
    //The function can return nullptr.
    //Don't foget clear this block after use.
    Connection* GetConnectionsArray() const;
    
    //The function create a new block of memory and return pointer to it.
    //The function return connections that have indicated point.
    //The function can return nullptr.
    //Don't foget clear this block after use.
    Connection* GetConnectionsArray(point_t point_number) const;
    
    //The function chec existing point with indicated number in the graph.
    bool IsHavePoint(point_t point_number) const;
    
    //Properties
    
    //The function chec that graph is basic.
    bool IsBasic() const;
    
    //The function chec that graph is connected.
    bool IsConnected() const;
    
    //The function chec that graph has cycle.
    bool IsHaveCycle() const;
    
    //The function chec that graph is thee.
    bool IsTree() const;
    
    //Properties
    
    void Paint();
    
    //The function sorts connections in the praph.
    void Sort();
    
    void operator=(Graph graph);
    
    //All nodes are same.
    bool operator==(Graph graph) const;
    
    //Some nodes are not same.
    bool operator!=(Graph graph) const;
    
    ~Graph();
};

std::ostream& operator<<(std::ostream& stream, Graph graph);

class Node
{
public:
    
    point_t color_id;
    float radius;   //radius of the point
    Vec2F position; //position of the point
    float text_size;//text size inside the point
    point_t number; //number of the point
    
    Node(const Node& point);
    
    Node(
        Point point = Point(),  //set number and color of the point
        Vec2F position = POINT_DEFAULT_POSITION,//set position of the point
        float radius = POINT_DEFAULT_RADIUS,    //set radius of the point
        float text_size = POINT_TEXT_SIZE_AUTO);//set text size of the point
    
    //The function returns poiter to string this number of the point.
    char* GetNumberAsTextC() const;
    
    //The function returns poiter to string (as integer array) this number of the point.
    int* GetNumberAsTextI() const;
    
    //The function returns count of signs in the value of the point.
    unsigned GetNumberTextLength() const;
    
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
    Vec2F p1_p;//pointer to pisition of point
    Vec2F p2_p;//pointer to pisition of point
    float shift_x;//shift to normal direction
    float shift_y;//shift to perpendicular direction
    
    PhysicConnection();
    
    PhysicConnection(const PhysicConnection& connection);
    
    PhysicConnection(
        const Node* p1,  //set pointer to first point position
        const Node* p2,  //set pointer to second point position
        float shift_x = CONNECTION_DEFAULT_DISTANCE_TO_POINT,   //set shift to normal direction
        float shift_y = CONNECTION_DEFAULT_SHIFT);              //set shift to perpendicular direction
    
    //The function returns segment between start and end points.
    Segment GetSegment() const;
    
    //The function returns indicated segment.
    Segment GetSegment(ConnectionTypes::segment_id_t segment_id) const;
    
    //The function updates positions of points.
    void Update();
    
    void operator=(PhysicConnection connection);
    
    ~PhysicConnection();
};

class PhysicGraph
{
protected:
    point_t nodes_count;
    point_t connections_count;
    float radius;
    
    Node* nodes;
    PhysicConnection* connections;
    
public:
    Vec2F position;
    //The constructor creates physic graph with out nodes and edges.
    PhysicGraph();
    
    //The constructor creates graph from input physic graph.
    PhysicGraph(const PhysicGraph& ph_graph);
    
    //The construtor creates graph from input graph.
    PhysicGraph(const Graph* connected_graph, Vec2F position);
    
    //The function returns indicated physic connection from graph.
    PhysicConnection GetConnection(point_t number) const;
    
    //The function returns count of physic connections in the physic graph.
    point_t GetConnectionsCount() const;
    
    //The function returns new array of physic connections.
    PhysicConnection* GetConnectionsArray() const;
    
    //The function returns pointer to existing array of physic connection in the physic graph.
    //If count of physic connections is zero, the funstion returns nullptr.
    const PhysicConnection* GetConnectionsArrayPointer() const;
    
    //The function returns indicated node from graph.
    Node GetNode(point_t number) const;
    
    //The function returns count of nodes in the physic graph.
    point_t GetNodesCount() const;
    
    //The function returns new array of nodes.
    Node* GetNodesArray() const;
    
    //The function return pointer to existing array of nodes in the physic graph.
    const Node* GetNodesArrayPointer() const;
        
    float GetRadius();
    
    void operator=(PhysicGraph ph_graph);
    
    //The destructor free memory from array of nodes and array of connections.
    ~PhysicGraph();
};








