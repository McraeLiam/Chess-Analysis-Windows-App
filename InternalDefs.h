#pragma once
#include <vector>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include <iostream>
#include <ctime>

int globalObjId = 0;

class Section
{
public:
    bool showBorder = true;
    D2D1_COLOR_F borderColor = D2D1::ColorF(1, 1, 1, 1);

    D2D1_RECT_F renderShape;

    Section(int x1, int y1, int x2, int y2)
    {
        renderShape = D2D1::RectF(x1, y1, x2, y2);
    }

    Section();

    void Resize(int x1, int y1, int x2, int y2)
    {
        renderShape = D2D1::RectF(x1, y1, x2, y2);
    }
};

class Object
{
public:
    int glId; //globalId


    bool operator==(const Object& rhs) const
    {
        return (glId == rhs.glId);
    };

    Object(int id)
    {
        glId = id;
    }
};

class GameArea
{
public:
    std::vector<Object*> children = {};

    bool showBorder = true;
    D2D1_COLOR_F borderColor = D2D1::ColorF(1, 1, 1, 1);
    D2D1_COLOR_F fillColor = D2D1::ColorF(1, 1, 1, 1);

    D2D1_RECT_F renderShape;

    GameArea(int x1, int y1, int x2, int y2)
    {
        renderShape = D2D1::RectF(x1, y1, x2, y2);
    }

    GameArea();

    void Resize(int x1, int y1, int x2, int y2)
    {
        renderShape = D2D1::RectF(x1, y1, x2, y2);
    }
    
private:
    void AddChild(Object appendObj)
    {
        children.push_back(&appendObj);
    }

    Object* AddChild() 
    {
        Object* newChild = new Object(globalObjId);
        children.push_back(newChild);
        ++globalObjId;
        return newChild;
    }

    void DeleteChild(Object removeChild) 
    {
        for (int i = 0; i < children.size(); ++i)
        {
            if (&removeChild == children[i])
            {
                children.erase(children.begin() + i);
            }
        }
    }

    ~GameArea() 
    {
        for (int i = 0; i < children.size(); ++i) 
        {
            delete children[0];
        }
    }
};