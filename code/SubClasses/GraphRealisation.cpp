#include "GraphRealisation.h"

Connection::Connection() :
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

point_t Connection::GetPoint1()
{
    return p1;
}

point_t Connection::GetPoint2()
{
    return p2;
}

point_t Connection::GetMaxPoint()
{
    if(p1 > p2)
    {
        return p1;
    }
    return p2;
}

point_t Connection::GetMinPoint()
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

Connection Connection::Swap()
{
    return Connection(p2, p1);
}

void Connection::SwapThis()
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

Connection::~Connection()
{
}

std::ostream& operator<<(std::ostream& stream, Connection connection)
{
    stream << connection.GetPoint1() << " -- " << connection.GetPoint2();
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

DirectConnection DirectConnection::Swap()
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

Graph::Graph(Connection* connections, point_t connections_count) :
    connections_count(connections_count)
{
    if(connections_count == 0)
    {
        this->connections = nullptr;
        return;
    }
    
    this->connections = new Connection[connections_count];
    
    for(point_t connection = 0; connection < connections_count; connection++)
    {
        this->connections[connection] = connections[connection];
    }
}

Graph::Graph(MatNI matrix) :
    connections_count(0)
{
    unsigned matrix_size = matrix.GetSize();
    if(matrix_size == 0)
    {
        connections_count = 0;
        std::cout << "ERROR::GRAPH::Input matrix is null." << std::endl;
        return;
    }
    unsigned values_count = matrix_size * matrix_size;
    int* values = matrix.GetValuesArray();
    
    for(unsigned i = 0; i < values_count; i++)
    {
        connections_count += values[i];
    }
    
    if(connections_count == 0)
    {
        connections = nullptr;
        return;
    }
    
    connections = new Connection[connections_count];
    
    point_t connection = 0;
    for(unsigned i = 0; i < values_count; i++)
    {
        if(values[i] != 0)
        {
            connections[connection] = Connection(i % matrix_size, i / matrix_size);
            connection++;
            if(connection == connections_count)
            {
                delete[] values;
                return;
            }
        }
    }
    
    delete[] values;
}

Graph::Graph(MatNI* matrix) :
    connections_count(0)
{
    unsigned matrix_size = matrix->GetSize();
    if(matrix_size == 0)
    {
        connections_count = 0;
        std::cout << "ERROR::GRAPH::Input matrix is null." << std::endl;
        return;
    }
    unsigned values_count = matrix_size * matrix_size;
    int* values = matrix->GetValuesArray();
    
    for(unsigned i = 0; i < values_count; i++)
    {
        connections_count += values[i];
    }
    
    if(connections_count == 0)
    {
        connections = nullptr;
        return;
    }
    
    connections = new Connection[connections_count];
    
    point_t connection = 0;
    for(unsigned i = 0; i < values_count; i++)
    {
        if(values[i] != 0)
        {
            connections[connection] = Connection(i % matrix_size, i / matrix_size);
            connection++;
            if(connection == connections_count)
            {
                delete[] values;
                return;
            }
        }
    }
    
    delete[] values;
}

bool Graph::Divide(Graph* return_graph)
{
    if(connections_count < 2)
    {
        return false;
    }
    
    bool* flags_array = new bool[connections_count];
    flags_array[0] = true;
    for(point_t f = 1; f < connections_count; f++)
    {
        flags_array[f] = false;
    }
    
    point_t next_con_c = 0;             //local connections count
    Connection* next_con_a = nullptr;   //local connection array
    for(point_t c = 0; c < connections_count; c++)
    {
        if(!flags_array[c])
        {
            next_con_c = GetNextConnectionsCount(c);
            if(next_con_c > 0)
            {
                next_con_a = GetNextConnectionsArray(c);
                for(point_t nc = 0; nc < next_con_c; nc++)
                {
                    for(point_t tc = 0; tc < connections_count; tc++)
                    {
                        if(flags_array[tc] && next_con_a[nc] == connections[tc])
                        {
                            flags_array[c] = true;
                            c = 0;              //return to start of the c cycle
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
    
    bool can_divide = false;
    
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
    
    for(point_t c = 0; c < connections_count; c++)
    {
        if(flags_array[c])
        {
            next_con_c++;
            next_con_a = (Connection*)realloc(next_con_a, sizeof(Connection) * next_con_c);
            next_con_a[next_con_c - 1] = connections[c];
        }
        else
        {
            new_con_c++;
        }
    }
    
    *return_graph = Graph(next_con_a, next_con_c);
    free(next_con_a);
    next_con_a = new Connection[new_con_c];
    new_con_c = 0;
    for(point_t c = 0; c < connections_count; c++)
    {
        if(!flags_array[c])
        {
            next_con_a[new_con_c] = connections[c];
            new_con_c++;
        }
    }
    delete[] flags_array;
    *this = Graph(next_con_a, new_con_c);
    delete[] next_con_a;
    return true;
}

point_t Graph::GetConnectionsCount()
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
    
    for(point_t c = 0; c < connections_count; c++)
    {
        if(connections[c].IsCollectPoint(p))
        {
            count++;
        }
    }
    return count;
}

Connection* Graph::GetNextConnectionsArray(point_t connection_index)
{
    Connection* return_array = new Connection[GetNextConnectionsCount(connection_index)];
    point_t next_con_c = 0;
    for(point_t c  = 0; c < connections_count; c++)
    {
        if(c != connection_index && 
            (
            connections[c].GetPoint1() == connections[connection_index].GetPoint1() ||
            connections[c].GetPoint2() == connections[connection_index].GetPoint1() ||
            connections[c].GetPoint1() == connections[connection_index].GetPoint2() ||
            connections[c].GetPoint2() == connections[connection_index].GetPoint2()
            ))
        {
            return_array[next_con_c] = connections[c];
            next_con_c++;
        }
    }
    return return_array;
}

point_t Graph::GetNextConnectionsCount(point_t connection_index)
{
    point_t next_con_c = 0; //next connections count
    for(point_t c  = 0; c < connections_count; c++)
    {
        if(c != connection_index && 
            (
            connections[c].GetPoint1() == connections[connection_index].GetPoint1() ||
            connections[c].GetPoint2() == connections[connection_index].GetPoint1() ||
            connections[c].GetPoint1() == connections[connection_index].GetPoint2() ||
            connections[c].GetPoint2() == connections[connection_index].GetPoint2()
            ))
        {
            next_con_c++;
        }
    }
    return next_con_c;
}

point_t* Graph::GetPointsArray()
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
    points_array[0] = connections[0].GetPoint1();
    if(points_array[0] != connections[0].GetPoint2())
    {
        points_array = (point_t*)realloc(points_array, sizeof(point_t) * 2lu);
        points_array[1] = connections[0].GetPoint2();
        points_count++;
    }
    point_t current = 0;
    for(point_t c = 1; c < connections_count; c++)
    {
        add = ADD_1 | ADD_2;
        for(point_t p = 0; p < points_count; p++)
        {
            if(points_array[p] == connections[c].GetPoint1())
            {
                add &= ADD_F - ADD_1;
            }
            if(points_array[p] == connections[c].GetPoint2())
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
            points_array[points_count - 1u] = connections[c].GetPoint1();
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[c].GetPoint2();
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

point_t Graph::GetPointsCount()
{
    if(connections_count == 0)
    {
        return 0;
    }
    
    uint8_t add = ADD_N;
    unsigned points_count = 1;
    point_t* points_array = (point_t*)malloc(sizeof(point_t));
    points_array[0] = connections[0].GetPoint1();
    if(points_array[0] != connections[0].GetPoint2())
    {
        points_array = (point_t*)realloc(points_array, sizeof(point_t) * 2lu);
        points_array[1] = connections[0].GetPoint2();
        points_count++;
    }
    point_t current = 0;
    for(point_t c = 1; c < connections_count; c++)
    {
        add = ADD_1 | ADD_2;
        for(point_t p = 0; p < points_count; p++)
        {
            if(points_array[p] == connections[c].GetPoint1())
            {
                add &= ADD_F - ADD_1;
            }
            if(points_array[p] == connections[c].GetPoint2())
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
            points_array[points_count - 1u] = connections[c].GetPoint1();
        }
        if(add & ADD_2)
        {
            points_count++;
            points_array = (point_t*)realloc(points_array, sizeof(point_t) * points_count);
            points_array[points_count - 1u] = connections[c].GetPoint2();
        }
    }
    
    free(points_array);
    
    return points_count;
}

Connection* Graph::GetConnectionsArray()
{
    if(connections_count == 0)
    {
        return nullptr;
    }
    
    Connection* return_connections_array = new Connection[connections_count];
    
    for(point_t c = 0; c < connections_count; c++)
    {
        return_connections_array[c] = connections[c];
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
    
    for(point_t c = 0, rc = 0; c < connections_count; c++)
    {
        if(connections[c].IsCollectPoint(p))
        {
            return_connections_array[rc] = connections[c];
            rc++;
            if(rc == count)
            {
                return return_connections_array;
            }
        }
    }
    
    return return_connections_array;
}

point_t Graph::GetMaxPoint()
{
    point_t max_point = 0;
    for(point_t c = 0; c < connections_count; c++)
    {
        if(max_point < connections[c].GetMaxPoint())
        {
            max_point = connections[c].GetMaxPoint();
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
    
    for(point_t c = 0; c < connections_count; c++)
    {
       if(connections[c].IsCollectPoint(p))
       {
           return true;
       }
    }
    return false;
}

bool Graph::IsBasic()
{
    for(point_t c = 0; c < connections_count; c++)
    {
        if(connections[c].GetPoint1() == connections[c].GetPoint2())
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

bool Graph::IsConnected()
{    
    bool* flags_array = new bool[connections_count];
    flags_array[0] = true;
    for(point_t c = 1; c < connections_count; c++)
    {
        flags_array[c] = false;
    }
    
    point_t next_con_c = 0;             //local connections count
    Connection* next_con_a = nullptr;   //local connection array
    for(point_t c = 0; c < connections_count; c++)
    {
        if(!flags_array[c])
        {
            next_con_c = GetNextConnectionsCount(c);
            if(next_con_c > 0)
            {
                next_con_a = GetNextConnectionsArray(c);
                for(point_t nc = 0; nc < next_con_c; nc++)
                {
                    for(point_t tc = 0; tc < connections_count; tc++)
                    {
                        if(flags_array[tc] && next_con_a[nc] == connections[tc])
                        {
                            flags_array[c] = true;
                            c = 0;              //return to start of the c cycle
                            nc = next_con_c;    //go out o thef nc cycle
                            break;              //go out of the tc cycle
                        }
                    }
                }
                delete[] next_con_a;
            }
        }
    }
    
    for(point_t c = 0; c < connections_count; c++)
    {
        if(!flags_array[c])
        {
            delete[] flags_array;
            return  false;
        }
    }
    
    delete[] flags_array;
    return true;
}

bool Graph::IsHaveCycle()
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

bool Graph::IsHaveCycleAsConnected()
{
    point_t con_c = 0;
    bool add_con = false;
    Connection* con_a = (Connection*)malloc(sizeof(Connection));
    for(point_t c = 0; c < connections_count; c++)
    {
        if(connections[c].GetPoint1() != connections[c].GetPoint2())
        {
            if(con_c == 0)
            {
                con_c++;
                con_a[0] = connections[c];
            }
            else
            {
                add_con = true;
                for(point_t nc = 0; nc < c; nc++)
                {
                    if(connections[nc] == connections[c])
                    {
                        add_con = false;
                        //std::cout << connections[nc] << " == " << connections[c] << std::endl;
                        break;
                    }
                }
                if(add_con)
                {
                    con_c++;
                    con_a = (Connection*)realloc(con_a, con_c * sizeof(Connection));
                    con_a[con_c - 1] = connections[c];
                }
            }
        }
    }
    free(con_a);
    
    return con_c >= GetPointsCount();
}

bool Graph::IsTree()
{
    return IsBasic() && IsConnected() && !IsHaveCycle();
}

void Graph::Sort()
{
    Connection temp;
    for(point_t c = 0; c < connections_count; c++)
    {
        if(connections[c].GetPoint1() > connections[c].GetPoint2())
        {
            connections[c].SwapThis();
        }
    }
    for(point_t c = 1; c < connections_count; c++)
    {
        if(connections[c - 1] > connections[c])
        {
            temp = connections[c - 1];
            connections[c - 1] = connections[c];
            connections[c] = temp;
            c = 0;
        }
    }
}

unsigned Graph::GetHight()
{
    if(connections_count == 0)
    {
        return 0;
    }
    point_t last = connections[0].GetPoint1();
    unsigned hight = 1;
    for(point_t c = 1; c < connections_count; c++)
    {
        if(last != connections[c].GetPoint1())
        {
            last = connections[c].GetPoint1();
            hight++;
        }
    }
    return hight;
}

unsigned Graph::GetWidth()
{
    if(connections_count == 0)
    {
        return 0;
    }
    point_t w_max = 0;
    point_t width = 0;
    point_t last = connections[0].GetPoint1();
    for(point_t c = 1; c < connections_count; c++)
    {
        if(last != connections[c].GetPoint1())
        {
            last = connections[c].GetPoint1();
            if(w_max < width)
            {
                w_max = width;
            }
            width = 0;
        }
        width++;
    }
    if(w_max < width)
    {
        return width + 1;
    }
    return w_max + 1;
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
    
    for(point_t c = 0; c < connections_count; c++)
    {
       connections[c] = graph.connections[c];
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
    if(connections_count != 0)
    {
        Connection* connections = graph.GetConnectionsArray();

        for(point_t c = 0; c < connections_count; c++)
        {
            stream  << connections[c] << std::endl;
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
        this->text_size = fminf(1.0f / (float)GetNumberTextLength() * POINT_NORMAL_TEXT_SIZE, POINT_MAX_TEXT_SIZE);
        return;
    }
    this->text_size = text_size;
}

int* Node::GetNumberAsTextI()
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

char* Node::GetNumberAsTextC()
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

unsigned Node::GetNumberTextLength()
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
    number = point.number;
    position = point.position;
    radius = point.radius;
    text_size = point.text_size;
}

Node::~Node()
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
    Update();
}

Segment PhysicConnection::GetSegment()
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

void PhysicConnection::Update()
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
    Vec2F y_vec = (*p2_p - *p1_p).Normalize().Perpendicular() * shift_y;
    Vec2F x_vec = (*p2_p - *p1_p).Normalize() * shift_x;
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
    Update();
}

PhysicConnection::~PhysicConnection()
{
}



























