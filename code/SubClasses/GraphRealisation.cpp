#include "GraphRealisation.h"

Connection::Connection(void) :
    p1(0),
    p2(0)
{
}

Connection::Connection(const Connection& connection) :
    p1(connection.p1),
    p2(connection.p2)
{
}

Connection::Connection(point_t p1, point_t p2) :
    p1(p1),
    p2(p2)
{
}


void Connection::ChangePoint1(point_t value)
{
    p1 = value;
}

void Connection::ChangePoint2(point_t value)
{
    p2 = value;
}

point_t Connection::GetPoint1(void)
{
    return p1;
}

point_t Connection::GetPoint2(void)
{
    return p2;
}

point_t Connection::GetMaxPoint(void)
{
    if(p1 > p2)
    {
        return p1;
    }
    return p2;
}

point_t Connection::GetMinPoint(void)
{
    if(p1 > p2)
    {
        return p2;
    }
    return p1;
}

bool Connection::IsCollectPoint(point_t p)
{
    return p1 == p || p2 == p;
}

Connection Connection::Swap(void)
{
    return Connection(p2, p1);
}

void Connection::SwapThis(void)
{
    point_t temp = p1;
    p1 = p2;
    p2 = temp;
}

void Connection::operator=(Connection connection)
{
    p1 = connection.p1;
    p2 = connection.p2;
}

bool Connection::operator==(Connection connection)
{
    return
        p1 == connection.p1 && p2 == connection.p2 ||
        connection.p1 == p2 && connection.p2 == p1;
}

bool Connection::operator!=(Connection connection)
{
    return
        !(p1 == connection.p1 && p2 == connection.p2 ||
        connection.p1 == p2 && connection.p2 == p1);
}

bool Connection::operator<(Connection connection)
{
    if(p1 == connection.p1)
    {
        return p2 < connection.p2;
    }
    return p1 < connection.p1;
}

bool Connection::operator>(Connection connection)
{
    if(p1 == connection.p1)
    {
        return p2 > connection.p2;
    }
    return p1 > connection.p1;
}

Connection::~Connection(void)
{
}

std::ostream& operator<<(std::ostream& stream, Connection connection)
{
    stream << connection.GetPoint1(void) << " -- " << connection.GetPoint2(void);
}



DirectConnection::DirectConnection(void) :
    Connection(void)
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

DirectConnection DirectConnection::Swap(void)
{
    return DirectConnection(p2, p1);
}

void DirectConnection::operator=(DirectConnection connection)
{
    p1 = connection.p1;
    p2 = connection.p2;
}

bool DirectConnection::operator==(DirectConnection connection)
{
    return
        p1 == connection.p1 && p2 == connection.p2;
}

bool DirectConnection::operator!=(DirectConnection connection)
{
    return
        !(p1 == connection.p1 && p2 == connection.p2);
}



Graph::Graph(void) :
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

Graph::Graph(Connection* connections, point_t connections_count)
{
    if(connections_count == 0)
    {
        this->connections = nullptr;
        return;
    }
    
    point_t* lone_points_arr = (point_t*)malloc(sizeof(point_t));///array of once point
    point_t lone_points_arr_s = 0;//lone points array size
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(
            connections[connection].GetPoint1(void) == 0
            connections[connection].GetPoint1(void) == connections[connection].GetPoint2(void))
        {
            std::cout << "Warning::Graph::0-0 connection detected.";
            for(point_t sec_conn = connection + 1; sec_conn < connections_count; sec_conn++)
            {
                connections[sec_conn - 1] = connections[sec_conn];
            }
            connections_count--;
            std::cout << "Connection number: " << (unsigned)connection << " remowed." << std::endl;
        }
        else if(connections[connection].GetPoint1(void) == 0)
        {
            bool add_new_lone_point = true;
            point_t sec_point = connections[connection].GetPoint2(void);
            
            for(point_t point = 0; point < lone_points_arr_s; point++)
            {
                if(lone_points_arr[point] == sec_point)
                {
                    std::cout << "Warning::Graph::More one same lone points detected." << std::endl;
                    for(point_t sec_conn = connection + 1; sec_conn < connections_count; sec_conn++)
                    {
                        connections[sec_conn - 1] = connections[sec_conn];
                    }
                    add_new_lone_point = false;
                    connections_count--;
                    std::cout << "Connection number: " << (unsigned)connection << " remowed." << std::endl;
                    break;
                }
            }
            if(add_new_lone_point)
            {
                lone_points_arr_s++;
                lone_points_arr = (point*)realloc(lone_points_arr, sizeof(point_t) * lone_points_arr_s);
            }
        }
        else if(connections[connection].GetPoint2(void) == 0)
        {
            bool add_new_lone_point = true;
            point_t sec_point = connections[connection].GetPoint1(void);
            
            for(point_t point = 0; point < lone_points_arr_s; point++)
            {
                if(lone_points_arr[point] == sec_point)
                {
                    std::cout << "Warning::Graph::More one same lone points detected." << std::endl;
                    for(point_t sec_conn = connection + 1; sec_conn < connections_count; sec_conn++)
                    {
                        connections[sec_conn - 1] = connections[sec_conn];
                    }
                    add_new_lone_point = false;
                    connections_count--;
                    std::cout << "Connection number: " << (unsigned)connection << " remowed." << std::endl;
                    break;
                }
            }
            if(add_new_lone_point)
            {
                lone_points_arr_s++;
                lone_points_arr = (point*)realloc(lone_points_arr, sizeof(point_t) * lone_points_arr_s);
            }
        }
    }
    
    free(lone_points_arr);
    
    if(connections_count == 0)
    {
        this->connections_count = connections_count;
        this->connections = new Connection[connections_count];
    }
    else
    {
        this->connections_count = 0;
        this->connections = nullptr;
        return;
    }
    
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        this->connections[connection] = connections[connection];
    }
}

bool Graph::Divide(Graph* return_graph)
{
    if(connections_count < 2)
    {
        return false;
    }
    
    bool* flags_array = new bool[connections_count];
    bool can_divide = true;
    
    for(point_t f = 0; f < connections_count; f++)
    {
        flags_array[f] = false;
    }
    for(point_t f = 0; f < connections_count; f++)
    {
        if(connections[f].GetPoint1(void) != 0 && connections[f].GetPoint2(void) != 0)
        {
            flags_array[f] = true;
            can_divide = false;
            break;
        }
    }
    
    if(can_divide)
    {
        *return_graph = Graph(&connections[connections_count - 1], 1);
        *this = Graph(connections, connections_count - 1);
        delete[] flags_array;
        return;
    }
    
    point_t next_con_c = 0;             //local connections count
    Connection* next_con_a = nullptr;   //local connection array
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(!flags_array[connection])
        {
            next_con_c = GetNextConnectionsCount(connection);
            if(next_con_c > 0)
            {
                next_con_a = GetNextConnectionsArray(connection);
                for(point_t nc = 0; nc < next_con_c; nc++)
                {
                    for(point_t tc = 0; tc < connections_count; tc++)
                    {
                        if(flags_array[tc] && next_con_a[nc] == connections[tc])
                        {
                            flags_array[connection] = true;
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
    
    next_con_a = (Connection*)malloc(sizeof(Connection));
    next_con_c = 0;
    
    for(point_t f = 0; f < connections_count; f++)
    {
        if(!flags_array[f])
        {
            can_divide = true;
            break;
        }
    }
    if(!can_divide)
    {
        return false;
    }
    
    point_t new_con_c = 0;
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(flags_array[connection])
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
    
    *return_graph = Graph(next_con_a, next_con_c);//create new graph from connected main graph part
    free(next_con_a);
    next_con_a = new Connection[new_con_c];
    new_con_c = 0;
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(!flags_array[connection])
        {
            next_con_a[new_con_c] = connections[connection];
            new_con_c++;
        }
    }
    delete[] flags_array;
    *this = Graph(next_con_a, new_con_c);
    delete[] next_con_a;
    return true;
}

point_t Graph::GetConnectionsCount(void)
{
    return connections_count;
}

point_t Graph::GetConnectionsCount(Connection connection)
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

point_t Graph::GetConnectionsCount(Connection* connection)
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

point_t Graph::GetConnectionsCount(point_t p)
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    point_t count = 0;
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsCollectPoint(p))
        {
            count++;
        }
    }
    return count;
}

Connection* Graph::GetNextConnectionsArray(point_t connection_index)
{
    Connection* return_array = new Connection[GetNextConnectionsCount(connection_index)];
    
    point_t next_con_c = 0; //next connections count
    
    if(connections[connection_index].GetPoint1(void) == 0)
    {
        if(connections[connection_index].GetPoint2(void) == 0)
        {//0-0
            return 0;
        }
        //0-k
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index
                (
                connections[connection].GetPoint1(void) == connections[connection_index].GetPoint2(void) ||
                connections[connection].GetPoint2(void) == connections[connection_index].GetPoint2(void)
                ))
            {
                return_array[next_con_c] = connections[connection];
                next_con_c++;
            }
        }
        return next_con_c;
    }
    if(connections[connection_index].GetPoint2(void) == 0)
    {//n-0
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index
                (
                connections[connection].GetPoint1(void) == connections[connection_index].GetPoint1(void) ||
                connections[connection].GetPoint2(void) == connections[connection_index].GetPoint1(void)
                ))
            {
                return_array[next_con_c] = connections[connection];
                next_con_c++;
            }
        }
        return next_con_c;
    }
    
    //n-k
    for(point_t connection  = 0; connection < connections_count; connection++)
    {
        if(connection != connection_index
            (
            connections[connection].GetPoint1(void) == connections[connection_index].GetPoint1(void) ||
            connections[connection].GetPoint2(void) == connections[connection_index].GetPoint1(void) ||
            connections[connection].GetPoint1(void) == connections[connection_index].GetPoint2(void) ||
            connections[connection].GetPoint2(void) == connections[connection_index].GetPoint2(void)
            ))
        {
            return_array[next_con_c] = connections[connection];
            next_con_c++;
        }
    }
    return next_con_c;
}

point_t Graph::GetNextConnectionsCount(point_t connection_index)
{
    point_t next_con_c = 0; //next connections count
    
    if(connections[connection_index].GetPoint1(void) == 0)
    {
        if(connections[connection_index].GetPoint2(void) == 0)
        {//0-0
            return 0;
        }
        //0-k
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index
                (
                connections[connection].GetPoint1(void) == connections[connection_index].GetPoint2(void) ||
                connections[connection].GetPoint2(void) == connections[connection_index].GetPoint2(void)
                ))
            {
                next_con_c++;
            }
        }
        return next_con_c;
    }
    if(connections[connection_index].GetPoint2(void) == 0)
    {//n-0
        for(point_t connection  = 0; connection < connections_count; connection++)
        {
            if(connection != connection_index
                (
                connections[connection].GetPoint1(void) == connections[connection_index].GetPoint1(void) ||
                connections[connection].GetPoint2(void) == connections[connection_index].GetPoint1(void)
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
        if(connection != connection_index
            (
            connections[connection].GetPoint1(void) == connections[connection_index].GetPoint1(void) ||
            connections[connection].GetPoint2(void) == connections[connection_index].GetPoint1(void) ||
            connections[connection].GetPoint1(void) == connections[connection_index].GetPoint2(void) ||
            connections[connection].GetPoint2(void) == connections[connection_index].GetPoint2(void)
            ))
        {
            next_con_c++;
        }
    }
    return next_con_c;
}

point_t* Graph::GetPointsArray(void)
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
    unsigned points_count = 1;
    point_t* points_array = (point_t*)malloc(sizeof(point_t));
    points_array[0] = connections[0].GetPoint1(void);
    if(points_array[0] != connections[0].GetPoint2(void))
    {
        points_array = (point_t*)realloc(points_array, sizeof(point_t) * 2lu);
        points_array[1] = connections[0].GetPoint2(void);
        points_count++;
    }
    point_t current = 0;
    for(point_t connection = 1; connection < connections_count; connection++)
    {
        add = ADD_1 | ADD_2;
        for(point_t p = 0; p < points_count; p++)
        {
            if(
                connections[connection].GetPoint1(void) != 0 &&
                points_array[p] == connections[connection].GetPoint1(void))
            {
                add &= ADD_F - ADD_1;
            }
            if(
                connections[connection].GetPoint2(void) != 0 &&
                points_array[p] == connections[connection].GetPoint2(void))
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
            points_array[points_count - 1u] = connections[connection].GetPoint1(void);
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[connection].GetPoint2(void);
        }
    }
    
    point_t* r_arr = new point_t[points_count];
    for(point_t p = 0; p < points_count; p++)
    {
        r_arr[p] = points_array[p];
    }
    free(points_array);
    
    return r_arr;
}

point_t Graph::GetPointsCount(void)
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    uint8_t add = ADD_N;
    unsigned points_count = 1;
    point_t* points_array = (point_t*)malloc(sizeof(point_t));
    points_array[0] = connections[0].GetPoint1(void);
    if(points_array[0] != connections[0].GetPoint2(void))
    {
        points_array = (point_t*)realloc(points_array, sizeof(point_t) * 2lu);
        points_array[1] = connections[0].GetPoint2(void);
        points_count++;
    }
    point_t current = 0;
    for(point_t connection = 1; connection < connections_count; connection++)
    {
        add = ADD_1 | ADD_2;
        for(point_t p = 0; p < points_count; p++)
        {
            if(
                connections[connection].GetPoint1(void) != 0 &&
                points_array[p] == connections[connection].GetPoint1(void))
            {
                add &= ADD_F - ADD_1;
            }
            if(
                connections[connection].GetPoint2(void) != 0 &&
                points_array[p] == connections[connection].GetPoint2(void))
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
            points_array[points_count - 1u] = connections[connection].GetPoint1(void);
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[connection].GetPoint2(void);
        }
    }
    
    free(points_array);
    
    return points_count;
}

Connection* Graph::GetConnectionsArray(void)
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

Connection* Graph::GetConnectionsArray(point_t p)
{
    if(connections_count == 0)
    {
        return nullptr;
    }
    
    point_t count = GetConnectionsCount(p);
    Connection* return_connections_array = new Connection[count];
    
    for(point_t connection = 0, rc = 0; connection < connections_count; connection++)
    {
        if(connections[connection].IsCollectPoint(p))
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

point_t Graph::GetMaxPoint(void)
{
    point_t max_point = 0;
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(max_point < connections[connection].GetMaxPoint(void))
        {
            max_point = connections[connection].GetMaxPoint(void);
        }
    }
    return max_point;
}

bool Graph::IsCollectPoint(point_t p)
{
    if(connections_count == 0)
    {
        return false;
    }
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
       if(connections[connection].IsCollectPoint(p))
       {
           return true;
       }
    }
    return false;
}

bool Graph::IsBasic(void)
{
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].GetPoint1(void) == connections[connection].GetPoint2(void))
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

bool Graph::IsConnected(void)
{    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(
            connection.GetPoint1(void) == 0 ||
            connection.GetPoint2(void) == 0)
        {
            return false;
        }
            
    }
    
    bool* flags_array = new bool[connections_count];
    flags_array[0] = true;
    for(point_t connection = 1; connection < connections_count; connection++)
    {
        flags_array[connection] = false;
    }
    
    point_t next_con_c = 0;             //local connections count
    Connection* next_con_a = nullptr;   //local connection array
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(!flags_array[connection])
        {
            next_con_c = GetNextConnectionsCount(connection);
            if(next_con_c > 0)
            {
                next_con_a = GetNextConnectionsArray(connection);
                for(point_t nc = 0; nc < next_con_c; nc++)
                {
                    for(point_t tc = 0; tc < connections_count; tc++)
                    {
                        if(flags_array[tc] && next_con_a[nc] == connections[tc])
                        {
                            flags_array[connection] = true;
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
        if(!flags_array[connection])
        {
            delete[] flags_array;
            return  false;
        }
    }
    
    delete[] flags_array;
    return true;
}

bool Graph::IsHaveCycle(void)
{
    point_t points_count = GetPointsCount(void);
    if(points_count < 3 || connections_count < 3)
    {
        return false;
    }
    
    Graph clone_graph = *this;
    Graph new_graph;
    
    while(clone_graph.Divide(&new_graph))
    {
        if(new_graph.IsHaveCycleAsConnected(void))
        {
            return true;
        }
    }
    return clone_graph.IsHaveCycleAsConnected(void);
}

bool Graph::IsHaveCycleAsConnected(void)
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
        if(connections[connection].GetPoint1(void) != connections[connection].GetPoint2(void))
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
                        //std::cout << connections[nc] << " == " << connections[connection] << std::endl;
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
    
    return con_c >= GetPointsCount(void);
}

bool Graph::IsTree(void)
{
    return IsBasic(void) && IsConnected(void) && !IsHaveCycle(void);
}

void Graph::Sort(void)
{
    Connection temp;
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        if(connections[connection].GetPoint1(void) > connections[connection].GetPoint2(void))
        {
            connections[connection].SwapThis(void);
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

Graph::~Graph(void)
{
    if(connections_count > 0)
    {
        delete[] connections;
    }
}

std::ostream& operator<<(std::ostream& stream, Graph graph)
{
    point_t connections_count = graph.GetConnectionsCount(void);
    if(connections_count != 0)
    {
        Connection* connections = graph.GetConnectionsArray(void);

        for(point_t connection = 0; connection < connections_count; connection++)
        {
            stream  << connections[connection] << std::endl;
        }
        delete[] connections;
        std::cout << "";
    }
}



Node::Node(const Node& point) :
    number(point.number),
    position(point.position),
    radius(point.radius),
    text_size(point.text_size)
{
}

Node::Node(point_t number, Vec2F position, float radius, float text_size) :
    number(number),
    position(position),
    radius(radius)
{
    if(text_size != text_size)
    {
        this->text_size = fminf(1.0f / (float)GetNumberTextLength(void) * POINT_NORMAL_TEXT_SIZE, POINT_MAX_TEXT_SIZE);
        return;
    }
    this->text_size = text_size;
}

int* Node::GetNumberAsTextI(void)
{
    int* text;
    if(number == 0)
    {
        text = new int[2];
        text[0] = '0';
        text[1] = '\0';
    }
    unsigned text_length = GetNumberTextLength(void);
    
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

char* Node::GetNumberAsTextC(void)
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

unsigned Node::GetNumberTextLength(void)
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

void Node::Update(void)
{
    if(text_size != text_size)
    {
        text_size = fminf(1.0f / (float)GetNumberTextLength(void) * POINT_NORMAL_TEXT_SIZE, POINT_MAX_TEXT_SIZE);
    }
}

void Node::operator=(Node point)
{
    number = point.number;
    position = point.position;
    radius = point.radius;
    text_size = point.text_size;
}

Node::~Node(void)
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

PhysicConnection::PhysicConnection(Node* point_1, Node* point_2, float shift_x, float shift_y) :
    p1_p(&point_1->position),
    p2_p(&point_2->position),
    shift_x(shift_x),
    shift_y(shift_y)
{    
    Update(void);
}

Segment PhysicConnection::GetSegment(void)
{
    return Segment(p1_p, p2_p, true);
}

Segment PhysicConnection::GetSegment(ConnectionTypes::segment_id_t segment_id)
{
    if(segment_id == CONNECTION_SEGMENT_ID_START)
    {
        return Segment(p1_p, &p1, true);
    }
    if(segment_id == CONNECTION_SEGMENT_ID_END)
    {
        return Segment(&p2, p2_p, true);
    }
    return Segment(&p1, &p2, true);
}

void PhysicConnection::Update(void)
{
    if(p1_p == nullptr || p2_p == nullptr)
    {
        return;
    }
    if(p1_p == p2_p)
    {
        p1 = *p1_p + Vec2F(-shift_y, shift_x).Rotate(CONNECTION_LOOP_ROTATE);
        p2 = *p1_p + Vec2F(shift_y, shift_x).Rotate(CONNECTION_LOOP_ROTATE);
        return;
    }
    Vec2F y_vec = (*p2_p - *p1_p).Normalize(void).Perpendicular(void) * shift_y;
    Vec2F x_vec = (*p2_p - *p1_p).Normalize(void) * shift_x;
    p1 = *p1_p + x_vec + y_vec;
    p2 = *p2_p - x_vec + y_vec;
}

void PhysicConnection::operator=(PhysicConnection connection)
{
    p1_p = connection.p1_p;
    p2_p = connection.p2_p;
    p1 = connection.p1;
    p2 = connection.p2;
    shift_y = connection.shift_y;
    shift_x = connection.shift_x;
    Update(void);
}

PhysicConnection::~PhysicConnection(void)
{
}



PhysicGraph::PhysicGraph(void) :
    connections(nullptr),
    connections_count(0),
    nodes(nullptr),
    nodes_count(0)
{
}

PhysicGraph::PhysicGraph(const PhysicGraph& ph_graph) :
    connections_count(ph_graph.connections_count),
    nodes_count(ph_graph.nodes_count),
    connections((connections_count == 0) ? nullptr : new PhysicConnection[connections_count]),
    nodes((nodes_count == 0) ? nullptr : new Node[nodes_count])
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

PhysicGraph::PhysicGraph(const Graph* graph, Vec2F position)
{    
    GraphTypes::point_t points_count = graph.GetPointsCount(void);
    GraphTypes::point_t connections_count = graph.GetConnectionsCount(void);
    if(points_count == 0 || connections_count == 0)
    {
        this->nodes_count = 0;
        this->connections_count = 0;
        this->nodes = nullptr;
        this->connection = nullptr;
        return;
    }
    
    this->nodes = new Node[points_count];
    
    Connection* c_arr = graph.GetConnectionsArray(void);
    GraphTypes::point_t* p_arr = graph.GetPointsArray(void);
    
    GraphTypes::point_t last_point = c_arr[0].GetPoint1(void);
    
    Connection last_con = c_arr[0];
    
    unsigned max_times = 0; //maximun count of adjacent edges that connect same two nodes.
    
    for(unsigned connection = 1, times = 0; connection < connections_count; connection++)
    {
        if(c_arr[connection] == last_con)
        {
            times++;
            if(max_times < times)
            {
                max_times = times;
            }
        }
    }
    
    
    Vec2F radius_vector = Vec2F((float)points_count * PH_NODE_POINT_RADIUS * 2.0f * (1.0f + (float)max_times * PH_GRAPH_PH_TIMES_PARAMETER), 0.0f);//node's position

    this->nodes[0] = Node(
        p_arr[0],                                   //point id
        radius_vector + position * PH_GRAPH_CELL_SIZE,       //point position
        PH_NODE_POINT_RADIUS);                              //point radius
    
    for(GraphTypes::point_t point = 0; point < points_count; point++)
    {
        this->nodes[point] = Node(
            p_arr[point],
            radius_vector.Rotate((float)point / (float)points_count * M_PI * 2.0f),
            PH_NODE_POINT_RADIUS);
    }
    
    this->connections = new PhysicConnection[this->connections_count];
    this->connections[0] = PhysicConnection(&this->nodes[0], &this->nodes[0], 0.0f, 0.0f);
    
    unsigned shift = 0;
    GraphTypes::point_t p1_id;
    GraphTypes::point_t p2_id;
    last_con = c_arr[0];
    float dist;
        
    for(GraphTypes::point_t connection = 1; connection < connections_count; connection++)
    {
        p1_id = points_count;
        p2_id = points_count;
        for(GraphTypes::point_t p = 0; p < points_count; p++)
        {
            if(c_arr[connection].GetPoint1(void) == p_arr[p])
            {
                p1_id = p;
            }
            if(c_arr[connection].GetPoint2(void) == p_arr[p])
            {
                p2_id = p;
            }
            if(p1_id != points_count && p2_id != points_count)
            {
                break;
            }
        }
        
        if(last_con != c_arr[connection])
        {
            last_con = c_arr[connection];
            shift = 0;
        }
        else if(connection > 0)
        {
            shift++;
        }
        
        dist = this->nodes[p1_id].position.GetDistance(this->nodes[p2_id].position);
        this->connections[connection] = PhysicConnection(
            &this->nodes[p1_id],    //first point
            &this->nodes[p2_id],    //second point
            (p1_id != p2_id) ? 
                 dist / 2.0f: 
                PH_GRAPH_CELL_SIZE_2 + shift * PH_GRAPH_CELL_SIZE,      //shift_x
            (p1_id != p2_id) ? 
                shift * PH_NODE_POINT_RADIUS * PH_GRAPH_PH_TIMES_PARAMETER : 
                PH_GRAPH_CELL_SIZE_2 + sqrt(shift * PH_GRAPH_CELL_SIZE));   //shift_y
    }
    
    delete[] c_arr;
    delete[] p_arr;
}

PhysicConnection PhysicGraph::GetPhysicConnection(point_t number) const
{
    if(number < connections_count)
    {
        return connections[number];
    }
    return PhysicConnection(void);
}

point_t PhysicGraph::GetPhysicConnectionsCount(void) const
{
    return connections_count;
}

PhysicConnection* PhysicGraph::GetConnectionsArray(void) const
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

const PhysicConnection* PhysicGraph::GetConnectionsArrayPointer(void) const
{
    return connections;
}

Node PhysicGraph::GetNode(point_t number) const
{
    if(number < nodes_count)
    {
        return nodes[number];
    }
    return Node(void);
}

point_t PhysicGraph::GetNodesCount(void) const
{
    return nodes_count;
}

Node* PhysicGraph::GetNodesArray(void) const
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

const Node* PhysicGraph::GetNodesArrayPointer(void) const
{
    return nodes;
}

void operator=(PhysicGraph ph_graph]
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
    nodes = new Nodep[nodes_count];
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
    connections = new Nodep[connections_count];
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        connections[connection] = ph_graph.connections[connection];
    }
}

~PhysicGraph(void)
{
    if(nodes_count > 0)
    {
        free(nodes);
    }
    
    if(connections_count > 0)
    {
        free(connections);
    }
}
































































