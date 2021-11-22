#include "GraphRealisation.h"


Point::Point() :
    number(0),
    color(0)
{
}

void Point::operator=(Point point)
{
    color = point.color;
    number = point.number;
}

Point::~Point()
{
}



Connection::Connection() :
    p1(0),
    p2(0)
{
}

Connection::Connection(const Connection& connection) :
    p1(connection.p1),
    p2(connection.p2),
    c1(connection.c1),
    c2(connection.c2)
{
}

Connection::Connection(point_t p1, point_t p2, color_t c1, color_t c2) :
    p1(p1),
    p2(p2),
    c1(c1),
    c2(c2)
{
}

void Connection::ChangeColor1(point_t value)
{
    c1 = value;
}

void Connection::ChangeColor2(point_t value)
{
    c2 = value;
}

void Connection::ChangePoint1(point_t value)
{
    p1 = value;
}

void Connection::ChangePoint2(point_t value)
{
    p2 = value;
}

point_t Connection::GetColor1() const
{
    return c1;
}

point_t Connection::GetColor2() const
{
    return c2;
}

point_t Connection::GetPoint1() const
{
    return p1;
}

point_t Connection::GetPoint2() const
{
    return p2;
}

point_t Connection::GetMaxPoint() const
{
    if(p1 > p2)
    {
        return p1;
    }
    return p2;
}

point_t Connection::GetMinPoint() const
{
    if(p1 > p2)
    {
        return p2;
    }
    return p1;
}

bool Connection::IsHavePoint(point_t p) const
{
    return p1 == p || p2 == p;
}

bool Connection::IsHaveSamePoint(const Connection* connection) const
{
    return
        p1 != 0 && (p1 == connection->p1 || p1 == connection->p2) ||
        p2 != 0 && (p2 == connection->p1 || p2 == connection->p2);
}

bool Connection::IsConnectOnePoint() const
{
    return p1 == p2 && p1 != 0;
}

bool Connection::IsPointsHaveSameColor() const
{
    return c1 == c2 && c1 != 0;
}

Connection Connection::Swap() const
{
    return Connection(p2, p1);
}

void Connection::SwapThis()
{
    point_t temp_n = p1;
    color_t temp_c = c1;
    p1 = p2;
    p2 = temp_n;
    c1 = c2;
    c2 = temp_c;
}

void Connection::operator=(Connection connection)
{
    p1 = connection.p1;
    p2 = connection.p2;
    c1 = connection.c1;
    c2 = connection.c2;
}

bool Connection::operator==(Connection connection) const
{
    return
        p1 == connection.p1 && p2 == connection.p2 ||
        connection.p1 == p2 && connection.p2 == p1;
}

bool Connection::operator!=(Connection connection) const
{
    return
        !(p1 == connection.p1 && p2 == connection.p2 ||
        connection.p1 == p2 && connection.p2 == p1);
}

bool Connection::operator<(Connection connection) const
{
    if(p1 == connection.p1)
    {
        return p2 < connection.p2;
    }
    return p1 < connection.p1;
}

bool Connection::operator>(Connection connection) const
{
    if(p1 == connection.p1)
    {
        return p2 > connection.p2;
    }
    return p1 > connection.p1;
}

Connection::~Connection()
{
}

std::ostream& operator<<(std::ostream& stream, Connection connection)
{
    stream << connection.GetPoint1() << " -- " << connection.GetPoint2() << " | "
        << connection.GetColor1() << " -- " << connection.GetColor2();
}



DirectConnection::DirectConnection() :
    Connection()
{
}

DirectConnection::DirectConnection(const DirectConnection& connection) :
    Connection(connection.p1, connection.p2)
{
}

DirectConnection::DirectConnection(point_t p1, point_t p2) :
    Connection(p1, p2)
{
}

DirectConnection DirectConnection::Swap() const
{
    return DirectConnection(p2, p1);
}

void DirectConnection::operator=(DirectConnection connection)
{
    p1 = connection.p1;
    p2 = connection.p2;
    c1 = connection.c1;
    c2 = connection.c2;
}

bool DirectConnection::operator==(DirectConnection connection) const
{
    return
        p1 == connection.p1 && p2 == connection.p2;
}

bool DirectConnection::operator!=(DirectConnection connection) const
{
    return
        !(p1 == connection.p1 && p2 == connection.p2);
}



Graph::Graph() :
    connections_count(0),
    connections(nullptr)
{
}

Graph::Graph(const Graph& graph) :
    connections_count(graph.connections_count)
{
    if(connections_count == 0)
    {
        connections = nullptr;
        return;
    }
    
    connections = new Connection[connections_count];
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        connections[connection] = graph.connections[connection];
    }
}

Graph::Graph(Connection* edges, point_t edges_count) :
    connections_count(0)
{    
    if(edges_count == 0)
    {
        this->connections = nullptr;
        return;
    }
    
    //The array stors flags that designate will it element of array transmit to graph.
    bool* flags = new bool[edges_count];
    for(point_t flag = 0; flag < edges_count; flag++)
    {
        flags[flag] = false;
    }
    
    for(point_t edge = 0; edge < edges_count; edge++)
    {
        if(
            edges[edge].GetPoint1() == 0 && edges[edge].GetPoint2() != 0 ||
            edges[edge].GetPoint1() != 0 && edges[edge].GetPoint2() == 0)
        {
            bool add_con = true;
            for(point_t edge2 = 0; edge2 < edge; edge2++)
            {
                if(edges[edge] == edges[edge2])
                {
                    std::cout << "WARNING::Lone point redefenition in edge " << edge + 1
                        << ". Last definition in edge " << edge2 + 1 << "." << std::endl;
                    add_con = false;
                }
                else if(edges[edge].IsHaveSamePoint(&edges[edge2]))
                {
                    std::cout << "WARNING::Point in edge " << edge + 1
                        << " set as lone but it was connected in edge " << edge2 + 1 << "." << std::endl;
                    add_con = false;
                }
            }
            if(add_con)
            {
                flags[edge] = true;
            }
        }
        else if(edges[edge].GetPoint1() != 0 && edges[edge].GetPoint2() != 0)
        {
            for(point_t edge2 = 0; edge2 < edge; edge2++)
            {
                if(
                    edges[edge].IsHaveSamePoint(&edges[edge2]) &&
                    edges[edge2].IsHavePoint(0))
                    //First edge and second edge have one same point and second edge has one zero point.
                {
                    std::cout << "WARNING::Point in edge " << edge2 + 1
                        << " set as lone but it is connected in edge " << edge + 1 << "." << std::endl;
                    flags[edge2] = false;
                }
            }
            flags[edge] = true;
        }
        else if(edges[edge].GetPoint1() == 0 && edges[edge].GetPoint2() == 0)
        {
            std::cout << "WARNING::Edge number " << edge + 1 << " is void." << std::endl;
            flags[edge] = false;
        }
    }
    
    for(point_t flag = 0; flag < edges_count; flag++)
    {
        if(flags[flag])
        {
            this->connections_count++;
        }
    }
    
    this->connections = new Connection[this->connections_count];
    
    for(point_t edge = 0, connection = 0; edge < edges_count; edge++)
    {
        if(flags[edge])
        {
            this->connections[connection] = edges[edge];
            connection++;
        }
        else
        {
            std::cout << "Edge number " << edge + 1 << " is skiped." << std::endl;
        }
    }
}

bool Graph::CheckPaint() const
{
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsPointsHaveSameColor())
        {
            return false;
        }
    }
    return true;
}

bool Graph::Divide(Graph* return_graph)
{
    if(connections_count < 2)
    {
        return false;
    }
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsHavePoint(0))
        {
            if(return_graph != nullptr)
            {
                *return_graph = Graph(&connections[connection], 1);
            }
            
            Connection* next_con_a = new Connection[connections_count - 1];
            for(point_t c = 0; c < connection; c++)
            {
                next_con_a[c] = connections[c];
            }
            for(point_t c = connection + 1; c < connections_count; c++)
            {
                next_con_a[c - 1] = connections[c];
            }
            delete[] connections;
            connections = next_con_a;
            connections_count--;
            return true;
        }
    }
    
    bool* flags = new bool[connections_count];
    bool can_divide = false;
    
    flags[0] = true;
    for(point_t f = 1; f < connections_count; f++)
    {
        flags[f] = false;
    }
    
    point_t next_con_c = 0;             //local edges count
    Connection* next_con_a = nullptr;   //local edges array
    for(point_t connection = 0; connection < connections_count; connection++)//go over all edges
    {
        if(!flags[connection])//if edges not mark
        {
            next_con_c = GetNextConnectionsCount(connection);
            if(next_con_c > 0)
            {
                next_con_a = GetNextConnectionsArray(connection);
                for(point_t nc = 0; nc < next_con_c; nc++)//go over all "next edge"
                {
                    for(point_t tc = 0; tc < connections_count; tc++)//go over all edges
                    {
                        if(flags[tc] && next_con_a[nc] == connections[tc])
                        {
                            flags[connection] = true;
                            connection = 0;      //return to start of the connection cycle
                            nc = next_con_c;    //go out of the nc cycle
                            break;              //go out of the tc cycle
                        }
                    }
                }
                delete[] next_con_a;
            }
        }
    }
    
    next_con_a = (Connection*)malloc(sizeof(Connection));
    next_con_c = 0;//count of edges in new graph
    
    for(point_t f = 0; f < connections_count; f++)
    {
        if(!flags[f])
        {
            can_divide = true;
            break;
        }
    }
    if(!can_divide)
    {
        return false;
    }
    
    point_t new_con_c = 0;//count of edges in this graph
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(flags[connection])
        {
            next_con_c++;
            next_con_a = (Connection*)realloc(next_con_a, sizeof(Connection) * next_con_c);
            next_con_a[next_con_c - 1] = connections[connection];
        }
        else
        {
            new_con_c++;
        }
    }
    
    if(return_graph != nullptr)
    {
        *return_graph = Graph(next_con_a, next_con_c);//create new graph from connected main graph part
    }
    free(next_con_a);
    next_con_a = new Connection[new_con_c];
    new_con_c = 0;
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(!flags[connection])
        {
            next_con_a[new_con_c] = connections[connection];
            new_con_c++;
        }
    }
    delete[] flags;
    delete[] connections;
    connections = next_con_a;
    connections_count = new_con_c;
    return true;
}

point_t Graph::GetConnectionsCount() const
{
    return connections_count;
}

point_t Graph::GetConnectionsCount(Connection connection) const
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    point_t return_count = 0;
    
    for(point_t i = 0; i < connections_count; i++)
    {
        if(connections[i] == connection)
        {
            return_count++;
        }
    }
    
    return return_count;
}

point_t Graph::GetConnectionsCount(Connection* connection) const
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    point_t return_count = 0;
    
    for(point_t i = 0; i < connections_count; i++)
    {
        if(connections[i] == *connection)
        {
            return_count++;
        }
    }
    
    return return_count;
}

point_t Graph::GetConnectionsCount(point_t p) const
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    point_t count = 0;
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsHavePoint(p))
        {
            count++;
        }
    }
    return count;
}

Connection* Graph::GetNextConnectionsArray(point_t connection_index) const
{
    Connection* return_array = new Connection[GetNextConnectionsCount(connection_index)];
    
    point_t next_con_c = 0; //next connections count
    
    if(connections[connection_index].GetPoint1() == 0)
    {
        if(connections[connection_index].GetPoint2() == 0)
        {//0-0
            return 0;
        }
        //0-k
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index &&
                (
                connections[connection].GetPoint1() == connections[connection_index].GetPoint2() ||
                connections[connection].GetPoint2() == connections[connection_index].GetPoint2()
                ))
            {
                return_array[next_con_c] = connections[connection];
                next_con_c++;
            }
        }
        return return_array;
    }
    if(connections[connection_index].GetPoint2() == 0)
    {//n-0
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index &&
                (
                connections[connection].GetPoint1() == connections[connection_index].GetPoint1() ||
                connections[connection].GetPoint2() == connections[connection_index].GetPoint1()
                ))
            {
                return_array[next_con_c] = connections[connection];
                next_con_c++;
            }
        }
        return return_array;
    }
    
    //n-k
    for(point_t connection  = 0; connection < connections_count; connection++)
    {
        if(connection != connection_index &&
            (
            connections[connection].GetPoint1() == connections[connection_index].GetPoint1() ||
            connections[connection].GetPoint2() == connections[connection_index].GetPoint1() ||
            connections[connection].GetPoint1() == connections[connection_index].GetPoint2() ||
            connections[connection].GetPoint2() == connections[connection_index].GetPoint2()
            ))
        {
            return_array[next_con_c] = connections[connection];
            next_con_c++;
        }
    }
    return return_array;
}

point_t Graph::GetNextConnectionsCount(point_t connection_index) const
{
    point_t next_con_c = 0; //next connections count
    
    if(connections[connection_index].GetPoint1() == 0)
    {
        if(connections[connection_index].GetPoint2() == 0)
        {//0-0
            return 0;
        }
        //0-k
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index &&
                (
                connections[connection].GetPoint1() == connections[connection_index].GetPoint2() ||
                connections[connection].GetPoint2() == connections[connection_index].GetPoint2()
                ))
            {
                next_con_c++;
            }
        }
        return next_con_c;
    }
    if(connections[connection_index].GetPoint2() == 0)
    {//n-0
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index &&
                (
                connections[connection].GetPoint1() == connections[connection_index].GetPoint1() ||
                connections[connection].GetPoint2() == connections[connection_index].GetPoint1()
                ))
            {
                next_con_c++;
            }
        }
        return next_con_c;
    }
    
    //n-k
    for(point_t connection  = 0; connection < connections_count; connection++)
    {
        if(connection != connection_index &&
            (
            connections[connection].GetPoint1() == connections[connection_index].GetPoint1() ||
            connections[connection].GetPoint2() == connections[connection_index].GetPoint1() ||
            connections[connection].GetPoint1() == connections[connection_index].GetPoint2() ||
            connections[connection].GetPoint2() == connections[connection_index].GetPoint2()
            ))
        {
            next_con_c++;
        }
    }
    return next_con_c;
}

point_t* Graph::GetPointsArray() const
{
    if(connections_count == 0)
    {
        return nullptr;
    }
#define ADD_N   0x00    //0000 0000
#define ADD_1   0x01    //0000 0001
#define ADD_2   0x02    //0000 0010
#define ADD_F   0xFF    //1111 1111
    
    uint8_t add = ADD_N;
    unsigned points_count = 0;
    point_t* points_array = (point_t*)malloc(sizeof(point_t));
    point_t current = 0;
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        add =
            ((connections[connection].GetPoint1() != 0) ? ADD_1 : ADD_N) |
            (
                (connections[connection].GetPoint2() != 0 &&
                !connections[connection].IsConnectOnePoint()) ? 
                ADD_2 : ADD_N
            );
        for(point_t p = 0; p < points_count; p++)
        {
            if(add & ADD_1 && points_array[p] == connections[connection].GetPoint1())
            {
                add &= ADD_F - ADD_1;
            }
            if(add & ADD_2 && points_array[p] == connections[connection].GetPoint2())
            {
                add &= ADD_F - ADD_2;
            }
            if(!add)
            {
                break;
            }
        }
        if(add & ADD_1)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[connection].GetPoint1();
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[connection].GetPoint2();
        }
    }
    
    if(points_count == 0)
    {
        free(points_array);
        return nullptr;
    }
    
    point_t* r_arr = new point_t[points_count];
    for(point_t p = 0; p < points_count; p++)
    {
        r_arr[p] = points_array[p];
    }
    free(points_array);
    
    return r_arr;
}

Point* Graph::GetColoredPointsArray() const
{
    if(connections_count == 0)
    {
        return nullptr;
    }
#define ADD_N   0x00    //0000 0000
#define ADD_1   0x01    //0000 0001
#define ADD_2   0x02    //0000 0010
#define ADD_F   0xFF    //1111 1111
    
    uint8_t add = ADD_N;
    point_t points_count = 0;
    Point* points_array = (Point*)malloc(sizeof(Point));
    point_t current = 0;
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        add =
            ((connections[connection].GetPoint1() != 0) ? ADD_1 : ADD_N) |
            (
                (connections[connection].GetPoint2() != 0 &&
                !connections[connection].IsConnectOnePoint()) ? 
                ADD_2 : ADD_N
            );
        for(point_t p = 0; p < points_count; p++)
        {
            if(add & ADD_1 && points_array[p].number == connections[connection].GetPoint1())
            {
                add &= ADD_F - ADD_1;
            }
            if(add & ADD_2 && points_array[p].number == connections[connection].GetPoint2())
            {
                add &= ADD_F - ADD_2;
            }
            if(!add)
            {
                break;
            }
        }
        if(add & ADD_1)
        {
            points_count++;
            points_array = (Point*)realloc(points_array, sizeof(Point) * points_count);
            points_array[points_count - 1u].number = connections[connection].GetPoint1();
            points_array[points_count - 1u].color = connections[connection].GetColor1();
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (Point*)realloc(points_array, sizeof(Point) * points_count);
            points_array[points_count - 1u].number = connections[connection].GetPoint2();
            points_array[points_count - 1u].color = connections[connection].GetColor2();
        }
    }
    
    if(points_count == 0)
    {
        free(points_array);
        return nullptr;
    }
    
    Point* r_arr = new Point[points_count];
    for(point_t p = 0; p < points_count; p++)
    {
        r_arr[p] = points_array[p];
    }
    free(points_array);
    
    return r_arr;
}

point_t Graph::GetPointsCount() const
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    uint8_t add = ADD_N;
    unsigned points_count = 0;
    point_t* points_array = (point_t*)malloc(sizeof(point_t));
    point_t current = 0;
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        add =
            ((connections[connection].GetPoint1() != 0) ? ADD_1 : ADD_N) |
            (
                (connections[connection].GetPoint2() != 0 && 
                !connections[connection].IsConnectOnePoint()) ?
                ADD_2 : ADD_N
            );
        for(point_t p = 0; p < points_count; p++)
        {
            if(add & ADD_1 && points_array[p] == connections[connection].GetPoint1())
            {
                add &= ADD_F - ADD_1;
            }
            if(add & ADD_2 && points_array[p] == connections[connection].GetPoint2())
            {
                add &= ADD_F - ADD_2;
            }
            if(!add)
            {
                break;
            }
        }
        if(add & ADD_1)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[connection].GetPoint1();
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[connection].GetPoint2();
        }
    }
    
    free(points_array);
    
    return points_count;
}

Connection* Graph::GetConnectionsArray() const
{
    if(connections_count == 0)
    {
        return nullptr;
    }
    
    Connection* return_connections_array = new Connection[connections_count];
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        return_connections_array[connection] = connections[connection];
    }
    
    return return_connections_array;
}

Connection* Graph::GetConnectionsArray(point_t p) const
{
    if(connections_count == 0)
    {
        return nullptr;
    }
    
    point_t count = GetConnectionsCount(p);
    Connection* return_connections_array = new Connection[count];
    
    for(point_t connection = 0, rc = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsHavePoint(p))
        {
            return_connections_array[rc] = connections[connection];
            rc++;
            if(rc == count)
            {
                return return_connections_array;
            }
        }
    }
    
    return return_connections_array;
}

point_t Graph::GetMaxPoint() const
{
    point_t max_point = 0;
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(max_point < connections[connection].GetMaxPoint())
        {
            max_point = connections[connection].GetMaxPoint();
        }
    }
    return max_point;
}

bool Graph::IsHavePoint(point_t p) const
{
    if(connections_count == 0)
    {
        return false;
    }
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
       if(connections[connection].IsHavePoint(p))
       {
           return true;
       }
    }
    return false;
}

bool Graph::IsBasic() const
{
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].GetPoint1() == connections[connection].GetPoint2())
        {
            return false;
        }
    }
    for(point_t c1 = 0; c1 < connections_count; c1++)
    {
        for(point_t c2 = c1 + 1; c2 < connections_count; c2++)
        {
            if(connections[c1] == connections[c2])
            {
                return false;
            }
        }
    }
    return true;
}

bool Graph::IsConnected() const
{    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(
            connections[connection].GetPoint1() == 0 ||
            connections[connection].GetPoint2() == 0)
        {
            return false;
        }
            
    }
    
    bool* flags = new bool[connections_count];
    flags[0] = true;
    for(point_t connection = 1; connection < connections_count; connection++)
    {
        flags[connection] = false;
    }
    
    point_t next_con_c = 0;             //local connections count
    Connection* next_con_a = nullptr;   //local connection array
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(!flags[connection])
        {
            next_con_c = GetNextConnectionsCount(connection);
            if(next_con_c > 0)
            {
                next_con_a = GetNextConnectionsArray(connection);
                for(point_t nc = 0; nc < next_con_c; nc++)
                {
                    for(point_t tc = 0; tc < connections_count; tc++)
                    {
                        if(flags[tc] && next_con_a[nc] == connections[tc])
                        {
                            flags[connection] = true;
                            connection = 0;              //return to start of the connection cycle
                            nc = next_con_c;    //go out o thef nc cycle
                            break;              //go out of the tc cycle
                        }
                    }
                }
                delete[] next_con_a;
            }
        }
    }
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(!flags[connection])
        {
            delete[] flags;
            return  false;
        }
    }
    
    delete[] flags;
    return true;
}

bool Graph::IsHaveCycle() const
{
    point_t points_count = GetPointsCount();
    if(points_count < 3 || connections_count < 3)
    {
        return false;
    }
    
    Graph clone_graph = *this;
    Graph new_graph;
    
    while(clone_graph.Divide(&new_graph))
    {
        if(new_graph.IsHaveCycleAsConnected())
        {
            return true;
        }
    }
    return clone_graph.IsHaveCycleAsConnected();
}

bool Graph::IsHaveCycleAsConnected() const
{
    if(connections_count < 2)
    {
        return false;
    }
    
    point_t con_c = 0;
    bool add_con = false;
    Connection* con_a = (Connection*)malloc(sizeof(Connection));
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].GetPoint1() != connections[connection].GetPoint2())
        {
            if(con_c == 0)
            {
                con_c++;
                con_a[0] = connections[connection];
            }
            else
            {
                add_con = true;
                for(point_t nc = 0; nc < connection; nc++)
                {
                    if(connections[nc] == connections[connection])
                    {
                        add_con = false;
                        break;
                    }
                }
                if(add_con)
                {
                    con_c++;
                    con_a = (Connection*)realloc(con_a, con_c * sizeof(Connection));
                    con_a[con_c - 1] = connections[connection];
                }
            }
        }
    }
    free(con_a);
    
    return con_c >= GetPointsCount();
}

bool Graph::IsTree() const
{
    return IsBasic() && IsConnected() && !IsHaveCycle();
}

void Graph::Paint()
{    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsConnectOnePoint())
        {
            std::cout << "WARNING::Graph have a loop adge. The function finish." << std::endl;
            return;
        }
    }
    
    
    point_t points_count = GetPointsCount();
    point_t* p_arr = GetPointsArray();
    
    bool new_variant = false;
    
    Point* points_array = (Point*)malloc(sizeof(Point) * points_count);
    for(point_t point = 0; point < points_count; point++)
    {
        points_array[point].number = p_arr[point];
        points_array[point].color = 1;
    }
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        connections[connection].ChangeColor1(1);
        connections[connection].ChangeColor2(1);
    }
    delete[] p_arr;
    
    if(CheckPaint())
    {
        free(points_array);
        return;
    }
    
    for(color_t colors_count = 2; colors_count <= points_count;)
    {
        for(point_t point = 0; point < points_count; point++)
        {
            if(points_array[point].color < colors_count)
            {
                points_array[point].color++;//change color o next
                break;
            }
            else
            {
                points_array[point].color = 1;//return color to start
                if(point == points_count - 1)//if it is end point
                {
                    colors_count++;//increment count of colors
                }
            }
        }
        
        for(point_t point = 0; point < points_count; point++)
        {
            for(point_t connection = 0; connection < connections_count; connection++)
            {
                if(connections[connection].GetPoint1() == points_array[point].number)
                {
                    connections[connection].ChangeColor1(points_array[point].color);
                    if(!new_variant && points_array[point].color == colors_count)
                    {
                        new_variant = true;
                    }
                }
                if(connections[connection].GetPoint2() == points_array[point].number)
                {
                    connections[connection].ChangeColor2(points_array[point].color);
                    if(!new_variant && points_array[point].color == colors_count)
                    {
                        new_variant = true;
                    }
                }
            }
        }
        if(new_variant)
        {
            new_variant = false;
             
            if(CheckPaint())
            {
                free(points_array);
                return;
            }
        }
    }
    
    free(points_array);
}

void Graph::Sort()
{
    Connection temp;
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].GetPoint1() > connections[connection].GetPoint2())
        {
            connections[connection].SwapThis();
        }
    }
    for(point_t connection = 1; connection < connections_count; connection++)
    {
        if(connections[connection - 1] > connections[connection])
        {
            temp = connections[connection - 1];
            connections[connection - 1] = connections[connection];
            connections[connection] = temp;
            connection = 0;
        }
    }
}

void Graph::operator=(Graph graph)
{
    if(connections_count > 0)
    {
        delete[] connections;
    }
    connections_count = graph.connections_count;
    if(connections_count == 0)
    {
        connections = nullptr;
        return;
    }
    
    connections = new Connection[connections_count];
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
       connections[connection] = graph.connections[connection];
    }
}

Graph::~Graph()
{
    if(connections_count > 0)
    {
        delete[] connections;
    }
}

std::ostream& operator<<(std::ostream& stream, Graph graph)
{
    point_t connections_count = graph.GetConnectionsCount();
    std::cout << "Connections count: " << connections_count << std::endl;
    if(connections_count > 0)
    {
        Connection* connections = graph.GetConnectionsArray();

        for(point_t connection = 0; connection < connections_count; connection++)
        {
            stream  << connections[connection] << std::endl;
        }
        delete[] connections;
    }
    stream << "";//If this line not exists then program crash. I don't know why.
}



Node::Node(const Node& point) :
    color_id(point.color_id),
    number(point.number),
    position(point.position),
    radius(point.radius),
    text_size(point.text_size)
{
}

Node::Node(Point point, Vec2F position, float radius, float text_size) :
    color_id(point.color),
    number(point.number),
    position(position),
    radius(radius)
{
    if(text_size != text_size)
    {
        this->text_size = fminf(1.0f / (float)GetNumberTextLength() * POINT_NORMAL_TEXT_SIZE, POINT_MAX_TEXT_SIZE);
        return;
    }
    this->text_size = text_size;
}

int* Node::GetNumberAsTextI() const
{
    int* text;
    if(number == 0)
    {
        text = new int[2];
        text[0] = '0';
        text[1] = '\0';
    }
    unsigned text_length = GetNumberTextLength();
    
    text = new int[text_length + 1];
    point_t temp = number;
    
    for(unsigned i = text_length; i > 0; i--)
    {
        text[i - 1] = '0' + (temp % 10);
        temp /= 10;
    }
    text[text_length] = '\0';
    return text;
}

char* Node::GetNumberAsTextC() const
{
    char* text;
    if(number == 0)
    {
        text = new char[2];
        text[0] = '0';
        text[1] = '\0';
    }
    unsigned text_length = 0u;
    
    for(point_t n = number; n > 0; n /= 10)
    {
        text_length++;
    }
    
    text = new char[text_length + 1];
    point_t temp = number;
    
    for(unsigned i = text_length; i > 0; i--)
    {
        text[i - 1] = '0' + (temp % 10);
        temp /= 10;
    }
    text[text_length] = '\0';
}

unsigned Node::GetNumberTextLength() const
{
    if(number == 0)
    {
        return 1u;
    }
    unsigned l = 0u;
    for(point_t n = number; n > 0; n /= 10)
    {
        l++;
    }
    return l;
}

void Node::Update()
{
    if(text_size != text_size)
    {
        text_size = fminf(1.0f / (float)GetNumberTextLength() * POINT_NORMAL_TEXT_SIZE, POINT_MAX_TEXT_SIZE);
    }
}

void Node::operator=(Node point)
{
    color_id = point.color_id;
    number = point.number;
    position = point.position;
    radius = point.radius;
    text_size = point.text_size;
}

Node::~Node()
{
}



PhysicConnection::PhysicConnection() :
    p1_p(Vec2F()),
    p2_p(Vec2F()),
    p1(Vec2F()),
    p2(Vec2F()),
    shift_x(0),
    shift_y(0)
{
}

PhysicConnection::PhysicConnection(const PhysicConnection& connection) :
    p1_p(connection.p1_p),
    p2_p(connection.p2_p),
    p1(connection.p1),
    p2(connection.p2),
    shift_x(connection.shift_x),
    shift_y(connection.shift_y)
{
}

PhysicConnection::PhysicConnection(const Node* point_1, const Node* point_2, float shift_x, float shift_y) :
    p1_p(point_1->position),
    p2_p(point_2->position),
    shift_x(shift_x),
    shift_y(shift_y)
{    
    Update();
}

Segment PhysicConnection::GetSegment() const
{
    return Segment(p1_p, p2_p, true);
}

Segment PhysicConnection::GetSegment(ConnectionTypes::segment_id_t segment_id) const
{
    if(segment_id == CONNECTION_SEGMENT_ID_START)
    {
        return Segment(p1_p, p1, true);
    }
    if(segment_id == CONNECTION_SEGMENT_ID_END)
    {
        return Segment(p2, p2_p, true);
    }
    return Segment(p1, p2, true);
}

void PhysicConnection::Update()
{
    
    if((p1_p - p2_p).Length() < PH_NODE_POINT_RADIUS * 3.0f)
    {
        p1 = p1_p + Vec2F(-shift_y, shift_x).Rotate(CONNECTION_LOOP_ROTATE);
        p2 = p1_p + Vec2F(shift_y, shift_x).Rotate(CONNECTION_LOOP_ROTATE);
        return;
    }
    Vec2F y_vec = (p2_p - p1_p).Normalize().Perpendicular() * shift_y;
    Vec2F x_vec = (p2_p - p1_p).Normalize() * shift_x;
    p1 = p1_p + x_vec + y_vec;
    p2 = p2_p - x_vec + y_vec;
}

void PhysicConnection::operator=(PhysicConnection connection)
{
    p1_p = connection.p1_p;
    p2_p = connection.p2_p;
    p1 = connection.p1;
    p2 = connection.p2;
    shift_y = connection.shift_y;
    shift_x = connection.shift_x;
    Update();
}

PhysicConnection::~PhysicConnection()
{
}



PhysicGraph::PhysicGraph() :
    connections(nullptr),
    connections_count(0),
    nodes(nullptr),
    nodes_count(0),
    radius(0)
{
}

PhysicGraph::PhysicGraph(const PhysicGraph& ph_graph) :
    connections_count(ph_graph.connections_count),
    nodes_count(ph_graph.nodes_count),
    connections((connections_count == 0) ? nullptr : new PhysicConnection[connections_count]),
    nodes((nodes_count == 0) ? nullptr : new Node[nodes_count]),
    radius(ph_graph.radius)
{
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        connections[connection] = ph_graph.connections[connection];
    }
    
    for(point_t node = 0; node < nodes_count; node++)
    {
        nodes[node] = ph_graph.nodes[node];
    }
}

PhysicGraph::PhysicGraph(const Graph* graph, Vec2F graph_position) :
    connections_count(0)
{
    Graph sort_graph = *graph;
    sort_graph.Sort();
    GraphTypes::point_t points_count = sort_graph.GetPointsCount();
    GraphTypes::point_t connections_count = sort_graph.GetConnectionsCount();
    
    if(points_count == 0 || connections_count == 0)
    {
        this->nodes_count = 0;
        this->nodes = nullptr;
        this->connections = nullptr;
        
        return;
    }
    
    this->nodes = new Node[points_count];
    
    Point* p_arr = sort_graph.GetColoredPointsArray();
    //If connections count is zero then the function returns nullptr.
    Connection* c_arr = sort_graph.GetConnectionsArray();
    
    point_t max_times = 0; //maximun count of adjacent edges that connect same two nodes.
    
    Connection last_con = Connection(0, 0);    
    
    for(point_t connection = 0, times = 0; connection < connections_count; connection++)
    {
        if(c_arr[connection].IsHavePoint(0))
        {
            times = 0;
        }
        else
        {
            this->connections_count++;
            if(c_arr[connection] == last_con)
            {
                times++;
                if(max_times < times)
                {
                    max_times = times;
                }
            }
            else
            {
                last_con = c_arr[connection];
                times = 0;
            }
        }
    }
    
    this->radius = 
        (float)points_count * PH_NODE_POINT_RADIUS  +
        (float)max_times * PH_GRAPH_PH_TIMES_PARAMETER;
        
    Vec2F radius_vector = Vec2F(this->radius, 0.0f);//node's position
    
    this->radius += PH_NODE_POINT_RADIUS;
    
    for(GraphTypes::point_t point = 0; point < points_count; point++)
    {
        this->nodes[point] = Node(
            p_arr[point],
            radius_vector.Rotate((float)point / (float)points_count * M_PI * 2.0f) + graph_position,
            PH_NODE_POINT_RADIUS);
    }
    
    this->connections = new PhysicConnection[this->connections_count];
    
    unsigned shift = 0;
    GraphTypes::point_t p1_id;
    GraphTypes::point_t p2_id;
    last_con = Connection(0, 0);
    float dist;
        
    for(GraphTypes::point_t connection = 0, ph_connection = 0; connection < connections_count; connection++)
    {
        if(!c_arr[connection].IsHavePoint(0))
        {
            p1_id = points_count;
            p2_id = points_count;
            for(GraphTypes::point_t p = 0; p < points_count; p++)
            {
                if(c_arr[connection].GetPoint1() == p_arr[p].number)
                {
                    p1_id = p;
                }
                if(c_arr[connection].GetPoint2() == p_arr[p].number)
                {
                    p2_id = p;
                }
                if(p1_id < points_count && p2_id < points_count)
                {
                    break;
                }
            }
            
            if(last_con != c_arr[connection])
            {
                last_con = c_arr[connection];
                shift = 0;
            }
            else
            {
                shift++;
            }
            
            dist = this->nodes[p1_id].position.GetDistance(this->nodes[p2_id].position);
            
            this->connections[ph_connection] = PhysicConnection(
                &this->nodes[p1_id],    //first point
                &this->nodes[p2_id],    //second point
                (p1_id != p2_id) ? 
                    dist / 2.0f :
                    PH_GRAPH_CELL_SIZE_2 + shift * PH_GRAPH_CELL_SIZE,      //shift_x
                (p1_id != p2_id) ? 
                    shift * PH_NODE_POINT_RADIUS * PH_GRAPH_PH_TIMES_PARAMETER : 
                    PH_GRAPH_CELL_SIZE_2 + sqrt(shift * PH_GRAPH_CELL_SIZE));   //shift_y
            
            ph_connection++;
        }
    }
    
    delete[] c_arr;
    delete[] p_arr;
    this->nodes_count = points_count;
}

PhysicConnection PhysicGraph::GetConnection(point_t number) const
{
    if(number < connections_count)
    {
        return connections[number];
    }
    return PhysicConnection();
}

point_t PhysicGraph::GetConnectionsCount() const
{
    return connections_count;
}

PhysicConnection* PhysicGraph::GetConnectionsArray() const
{
    if(connections_count == 0)
    {
        return nullptr;
    }
    
    PhysicConnection* return_connections_array = new PhysicConnection[connections_count];
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        return_connections_array[connection] = connections[connection];
    }
    return return_connections_array;
}

const PhysicConnection* PhysicGraph::GetConnectionsArrayPointer() const
{
    return connections;
}

Node PhysicGraph::GetNode(point_t number) const
{
    if(number < nodes_count)
    {
        return nodes[number];
    }
    return Node();
}

point_t PhysicGraph::GetNodesCount() const
{
    return nodes_count;
}

Node* PhysicGraph::GetNodesArray() const
{
    if(nodes_count == 0)
    {
        return nullptr;
    }
    
    Node* return_nodes_array = new Node[nodes_count];
    for(point_t node = 0; node < nodes_count; node++)
    {
        return_nodes_array[node] = nodes[node];
    }
    return return_nodes_array;
}

const Node* PhysicGraph::GetNodesArrayPointer() const
{
    return nodes;
}

float PhysicGraph::GetRadius()
{
    return radius;
}

void PhysicGraph::operator=(PhysicGraph ph_graph)
{
    if(ph_graph.nodes_count == 0)
    {
        connections_count = 0;
        nodes_count = 0;
        connections = nullptr;
        nodes = nullptr;
        return;
    }
    
    nodes_count = ph_graph.nodes_count;
    nodes = new Node[nodes_count];
    for(point_t node = 0; node < nodes_count; node++)
    {
        nodes[node] = ph_graph.nodes[node];
    }
    
    if(ph_graph.connections == 0)
    {        
        connections_count = 0;
        connections = nullptr;
        return;
    }
    
    connections_count = ph_graph.connections_count;
    connections = new PhysicConnection[connections_count];
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        connections[connection] = ph_graph.connections[connection];
    }
    
    position = ph_graph.position;
    radius = ph_graph.radius;
}

PhysicGraph::~PhysicGraph()
{
    if(nodes_count > 0)
    {
        delete[] nodes;
    }
    
    if(connections_count > 0)
    {
        delete[] connections;
    }
}
































































