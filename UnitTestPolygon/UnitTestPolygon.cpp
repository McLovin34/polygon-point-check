#include "pch.h"
#include "CppUnitTest.h"

#include "../Polygon/Error.h"
#include "../Polygon/Validator.h"
#include "../Polygon/Point.h"
#include "../Polygon/Polygon.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{

    TEST_CLASS(ValidatorTests)
    {
    public:
        // signedArea
        TEST_METHOD(SignedArea_Triangle)
        {
            Validator v;
            std::vector<Point> a{ {0,0}, {1,0}, {0,2} };
            Assert::AreEqual(2LL, v.signedArea(a));
        }
        TEST_METHOD(SignedArea_Rectangle)
        {
            Validator v;
            std::vector<Point> b{ {0,0}, {1,0}, {1,1}, {0,1} };
            Assert::AreEqual(2LL, v.signedArea(b));
        }
        TEST_METHOD(SignedArea_Reversed)
        {
            Validator v;
            std::vector<Point> c{ {0,0}, {0,1}, {1,1}, {1,0} };
            Assert::AreEqual(-2LL, v.signedArea(c));
        }

        TEST_METHOD(Orientation_Clockwise)
        {
            Validator v;
            Point a{ 0,0 }, b{ 4,4 }, c{ 5,2 };
            // Теперь поворот из a→b→c — по часовой, и функция вернёт 1
            Assert::AreEqual(1, v.orientation(a, b, c));
        }

        TEST_METHOD(Orientation_CounterClockwise)
        {
            Validator v;
            Point a{ 0,0 }, b{ 4,4 }, c{ 2,5 };
            Assert::AreEqual(2, v.orientation(a, b, c));
        }
        TEST_METHOD(Orientation_Collinear)
        {
            Validator v;
            Point a{ 0,0 }, b{ 2,2 }, c{ 1,1 };
            Assert::AreEqual(0, v.orientation(a, b, c));
        }

        // onSegment
        TEST_METHOD(OnSegment_True)
        {
            Validator v;
            Point p{ 0,0 }, q{ 1,1 }, r{ 2,2 };
            Assert::IsTrue(v.onSegment(p, q, r));
        }
        TEST_METHOD(OnSegment_False)
        {
            Validator v;
            Point p{ 0,0 }, q{ 3,3 }, r{ 2,2 };
            Assert::IsFalse(v.onSegment(p, q, r));
        }

        // checkCollinearity
        TEST_METHOD(CheckCollinearity_True)
        {
            Validator v;
            Point a{ 0,0 }, b{ 2,2 }, c{ 1,1 };
            Assert::IsTrue(v.checkCollinearity(a, b, c));
        }
        TEST_METHOD(CheckCollinearity_False)
        {
            Validator v;
            Point a{ 0,0 }, b{ 2,2 }, d{ 1,2 };
            Assert::IsFalse(v.checkCollinearity(a, b, d));
        }

        // checkIntersection
        TEST_METHOD(CheckIntersection_Intersect)
        {
            Validator v;
            Point a1{ 0,0 }, a2{ 2,2 }, b1{ 0,2 }, b2{ 2,0 };
            Assert::IsTrue(v.checkIntersection(a1, a2, b1, b2));
        }
        TEST_METHOD(CheckIntersection_NoIntersect)
        {
            Validator v;
            Point c1{ 0,0 }, c2{ 1,1 }, d1{ 2,2 }, d2{ 3,3 };
            Assert::IsFalse(v.checkIntersection(c1, c2, d1, d2));
        }

        // checkVertexRange
        TEST_METHOD(CheckVertexRange_Within)
        {
            Validator v;
            Error err;
            Point p{ 999,999 };
            Assert::IsTrue(v.checkVertexRange(p, err, 1));
        }
        TEST_METHOD(CheckVertexRange_Outside)
        {
            Validator v;
            Error err;
            Point p{ 1000,0 };
            Assert::IsFalse(v.checkVertexRange(p, err, 2));
        }

        // checkVertexCount
        TEST_METHOD(CheckVertexCount_Min)
        {
            Validator v;
            Error err;
            Assert::IsTrue(v.checkVertexCount(3, err));
        }
        TEST_METHOD(CheckVertexCount_Max)
        {
            Validator v;
            Error err;
            Assert::IsTrue(v.checkVertexCount(1000, err));
        }
        TEST_METHOD(CheckVertexCount_TooFew)
        {
            Validator v;
            Error err;
            Assert::IsFalse(v.checkVertexCount(2, err));
        }
        TEST_METHOD(CheckVertexCount_TooMany)
        {
            Validator v;
            Error err;
            Assert::IsFalse(v.checkVertexCount(1001, err));
        }

        // checkDuplicateVertices
        TEST_METHOD(CheckDuplicateVertices_None)
        {
            Validator v;
            Error err;
            std::vector<Point> v1{ {0,0}, {1,1}, {2,2} };
            Assert::IsTrue(v.checkDuplicateVertices(v1, err));
        }
        TEST_METHOD(CheckDuplicateVertices_Exists)
        {
            Validator v;
            Error err;
            std::vector<Point> v2{ {0,0}, {1,1}, {0,0} };
            Assert::IsFalse(v.checkDuplicateVertices(v2, err));
        }

        // checkCorrectVertexOrder
        TEST_METHOD(CheckCorrectVertexOrder_CCW)
        {
            Validator v;
            Error err;
            std::vector<Point> ccw{ {0,0},{1,0},{1,1},{0,1} };
            Assert::IsTrue(v.checkCorrectVertexOrder(ccw, err));
        }
        TEST_METHOD(CheckCorrectVertexOrder_CW)
        {
            Validator v;
            Error err;
            std::vector<Point> cw{ {0,0},{0,1},{1,1},{1,0} };
            Assert::IsFalse(v.checkCorrectVertexOrder(cw, err));
        }
        TEST_METHOD(CheckCorrectVertexOrder_CollinearPoly)
        {
            Validator v;
            Error err;
            std::vector<Point> col{ {0,0},{1,1},{2,2} };
            Assert::IsFalse(v.checkCorrectVertexOrder(col, err));
            Assert::IsTrue(err.type == ErrorType::wrongOrder);
        }

        // checkPolygonShape — разные случаи
        TEST_METHOD(CheckPolygonShape_ConcaveSimple)
        {
            Validator v;
            Error err;
            std::vector<Point> v1{ {0,0},{2,1},{1,2},{2,3},{0,4} };
            Assert::IsTrue(v.checkPolygonShape(v1, err));
        }
        TEST_METHOD(CheckPolygonShape_Collinear)
        {
            Validator v;
            Error err;
            std::vector<Point> v2{ {0,0},{1,1},{2,2},{0,2} };
            Assert::IsFalse(v.checkPolygonShape(v2, err));
        }
        TEST_METHOD(CheckPolygonShape_SelfIntersecting)
        {
            Validator v;
            Error err;
            std::vector<Point> v3{ {0,0},{2,2},{0,2},{2,0} };
            Assert::IsFalse(v.checkPolygonShape(v3, err));
        }
        TEST_METHOD(CheckPolygonShape_Convex)
        {
            Validator v;
            Error err;
            std::vector<Point> v4{ {0,0},{1,0},{1,1},{0,1} };
            Assert::IsFalse(v.checkPolygonShape(v4, err));
        }

        // validate — комбинированные
        TEST_METHOD(Validate_AllValid)
        {
            Validator v;
            Error err;
            std::vector<Point> p{ {0,0},{2,1},{1,2},{2,3},{0,4} };
            Point tp{ 1,2 };
            Assert::IsTrue(v.validate(p, tp, err));
        }
        TEST_METHOD(Validate_ErrorVertexCount)
        {
            Validator v;
            Error err;
            std::vector<Point> p2{ {0,0},{1,1} };
            Assert::IsFalse(v.validate(p2, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::invalidVertexCount);
        }
        TEST_METHOD(Validate_ErrorDuplicate)
        {
            Validator v;
            Error err;
            std::vector<Point> p3{ {0,0},{1,1},{0,0} };
            Assert::IsFalse(v.validate(p3, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::duplicateVertex);
        }
        TEST_METHOD(Validate_ErrorOutOfRangeVertex)
        {
            Validator v;
            Error err;
            std::vector<Point> p4{ {0,0},{1001,0},{1,1} };
            Assert::IsFalse(v.validate(p4, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::coordinateOutOfRange);
        }
        TEST_METHOD(Validate_ErrorWrongOrder)
        {
            Validator v;
            Error err;
            std::vector<Point> p5{ {0,0},{0,1},{1,1},{1,0} };
            Assert::IsFalse(v.validate(p5, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::wrongOrder);
        }
        TEST_METHOD(Validate_ErrorConvexPolygon)
        {
            Validator v;
            Error err;
            std::vector<Point> sq{ {0,0},{1,0},{1,1},{0,1} };
            Assert::IsFalse(v.validate(sq, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::invalidPolygon);
        }
        TEST_METHOD(Validate_ErrorCollinear)
        {
            Validator v;
            Error err;
            std::vector<Point> p6{ {0,0},{1,1},{2,2},{0,2} };
            Assert::IsFalse(v.validate(p6, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::invalidPolygon);
        }
        TEST_METHOD(Validate_ErrorSelfIntersection)
        {
            Validator v;
            Error err;
            std::vector<Point> p7{ {0,0}, {4,0}, {1,4}, {3,4} };
            Assert::IsFalse(v.validate(p7, { 0,0 }, err));
            Assert::IsTrue(err.type == ErrorType::invalidPolygon);
        }
        TEST_METHOD(Validate_ErrorPointOutOfRange_Both)
        {
            Validator v;
            Error err;
            std::vector<Point> p9{ {0,0},{2,1},{1,2},{2,3},{0,4} };
            Assert::IsFalse(v.validate(p9, { 2000,2000 }, err));
            Assert::IsTrue(err.type == ErrorType::pointOutOfRange);
        }
        TEST_METHOD(Validate_ErrorPointOutOfRange_X)
        {
            Validator v;
            Error err;
            std::vector<Point> p{ {0,0},{2,1},{1,2},{2,3},{0,4} };
            Assert::IsFalse(v.validate(p, { 1000,0 }, err));
            Assert::IsTrue(err.type == ErrorType::pointOutOfRange);
        }
        TEST_METHOD(Validate_ErrorPointOutOfRange_Y)
        {
            Validator v;
            Error err;
            std::vector<Point> p{ {0,0},{2,1},{1,2},{2,3},{0,4} };
            Assert::IsFalse(v.validate(p, { 0,1000 }, err));
            Assert::IsTrue(err.type == ErrorType::pointOutOfRange);
        }
    };


    TEST_CLASS(PolygonTests)
    {
    public:
        // isValid cases
        TEST_METHOD(IsValid_ConcaveSimple)
        {
            Error err;
            std::vector<Point> v{ {0,0},{2,1},{1,2},{2,3},{0,4} };
            Assert::IsTrue(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_Concave_NegativeCoords)
        {
            Error err;
   
            std::vector<Point> v{
                { -2, -2 }, {  1, -2 }, {  1, -1 }, {  0, -1 }, {  0,  0 }, { -1,  0 }, { -1, -1 }, { -2, -1 }
            };
            Assert::IsTrue(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_InvalidVertexCount)
        {
            Error err;
            std::vector<Point> v{ {0,0},{1,1} };
            Assert::IsFalse(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_InvalidDuplicate)
        {
            Error err;
            std::vector<Point> v{ {0,0},{1,1},{0,0} };
            Assert::IsFalse(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_OutOfRangeVertex)
        {
            Error err;
            std::vector<Point> v{ {0,0},{1001,0},{1,1},{-1001,0},{1,-1} };
            Assert::IsFalse(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_WrongOrder)
        {
            Error err;
            std::vector<Point> v{ {0,0},{0,1},{1,1},{1,0} };
            Assert::IsFalse(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_Collinear)
        {
            Error err;
            std::vector<Point> v{ {0,0},{1,1},{2,2},{2,0} };
            Assert::IsFalse(Polygon(v).isValid(err));
        }
        TEST_METHOD(IsValid_SelfIntersecting)
        {
            Error err;
            std::vector<Point> v{ {0,0},{2,2},{0,2},{2,0} };
            Assert::IsFalse(Polygon(v).isValid(err));
        }

        TEST_METHOD(Contains_Triangle_Inside)
        {
            std::vector<Point> tri{ {0,0},{5,0},{2.5,5} };
            Assert::IsTrue(Polygon(tri).contains({ 2.5,2 }));
        }
        TEST_METHOD(Contains_Triangle_Outside)
        {
            std::vector<Point> tri{ {0,0},{5,0},{2.5,5} };
            Assert::IsFalse(Polygon(tri).contains({ 5,5 }));
        }
        TEST_METHOD(Contains_Rectangle_Corner)
        {
            std::vector<Point> rect{ {0,0},{4,0},{4,3},{0,3} };
            Assert::IsTrue(Polygon(rect).contains({ 4,3 }));
        }
        TEST_METHOD(Contains_Rectangle_OutsideDiagonal)
        {
            std::vector<Point> rect{ {0,0},{4,0},{4,3},{0,3} };
            Assert::IsFalse(Polygon(rect).contains({ 4.1,3.1 }));
        }
        TEST_METHOD(Contains_ConcaveU_InsideArm)
        {
            std::vector<Point> u{ {0,0},{4,0},{4,4},{3,4},{3,1},{1,1},{1,4},{0,4} };
            Assert::IsTrue(Polygon(u).contains({ 0.5,2 }));
        }
        TEST_METHOD(Contains_ConcaveU_InCavity)
        {
            std::vector<Point> u{ {0,0},{4,0},{4,4},{3,4},{3,1},{1,1},{1,4},{0,4} };
            Assert::IsFalse(Polygon(u).contains({ 2,2 }));
        }
        TEST_METHOD(Contains_Diamond_Inside)
        {
            std::vector<Point> d{ {2,0},{4,2},{2,4},{0,2} };
            Assert::IsTrue(Polygon(d).contains({ 2,2 }));
        }
        TEST_METHOD(Contains_Diamond_Outside)
        {
            std::vector<Point> d{ {2,0},{4,2},{2,4},{0,2} };
            Assert::IsFalse(Polygon(d).contains({ 3.5,3.5 }));
        }
        TEST_METHOD(Contains_HorizontalEdge_Point)
        {
            std::vector<Point> h{ {0,0},{5,0},{5,5},{0,5} };
            Assert::IsTrue(Polygon(h).contains({ 2.5,0 }));
        }
        TEST_METHOD(Contains_VerticalEdge_Point)
        {
            std::vector<Point> vEdge{ {0,0},{5,0},{5,5},{0,5} };
            Assert::IsTrue(Polygon(vEdge).contains({ 0,2.5 }));
        }
        // Additional coverage:

        TEST_METHOD(Contains_FractionalSquare_Inside)
        {
            std::vector<Point> sq{ {0.5,0.5},{2.5,0.5},{2.5,2.5},{0.5,2.5} };
            Assert::IsTrue(Polygon(sq).contains({ 1.5,1.5 }));
        }
        TEST_METHOD(Contains_FractionalSquare_Outside)
        {
            std::vector<Point> sq{ {0.5,0.5},{2.5,0.5},{2.5,2.5},{0.5,2.5} };
            Assert::IsFalse(Polygon(sq).contains({ 0.4,1.5 }));
        }
        TEST_METHOD(Contains_Trapezoid_Inside)
        {
            std::vector<Point> trap{ {0,0},{4,0},{3,3},{1,3} };
            Assert::IsTrue(Polygon(trap).contains({ 2,1 }));
        }
        TEST_METHOD(Contains_Trapezoid_Outside)
        {
            std::vector<Point> trap{ {0,0},{4,0},{3,3},{1,3} };
            Assert::IsFalse(Polygon(trap).contains({ 2,3.1 }));
        }
        TEST_METHOD(Contains_Pentagon_Inside)
        {
            std::vector<Point> pent{ {0,0},{2,1},{3,3},{1,4},{-1,2} };
            Assert::IsTrue(Polygon(pent).contains({ 1,2 }));
        }
        TEST_METHOD(Contains_Pentagon_Outside)
        {
            std::vector<Point> pent{ {0,0},{2,1},{3,3},{1,4},{-1,2} };
            Assert::IsFalse(Polygon(pent).contains({ 2.9,1.1 }));
        }
        TEST_METHOD(Contains_StarShape_Inside)
        {
            std::vector<Point> star{ {0,3},{1,1},{3,0},{1,-1},{0,-3},{-1,-1},{-3,0},{-1,1} };
            Assert::IsTrue(Polygon(star).contains({ 0,0 }));
        }
        TEST_METHOD(Contains_StarShape_Outside)
        {
            std::vector<Point> star{ {0,3},{1,1},{3,0},{1,-1},{0,-3},{-1,-1},{-3,0},{-1,1} };
            Assert::IsFalse(Polygon(star).contains({ 2,2 }));
        }
        TEST_METHOD(Contains_NegativeFractionalRectangle_Inside)
        {
            std::vector<Point> rect{ { -2.5f, -1.5f }, {  1.5f, -1.5f }, {  1.5f,  2.5f }, { -2.5f,  2.5f } };
            Assert::IsTrue(Polygon(rect).contains({ 0.0f, 0.5f }));
        }
        TEST_METHOD(Contains_NegativeFractionalRectangle_Outside)
        {
            std::vector<Point> rect{ { -2.5f, -1.5f }, {  1.5f, -1.5f }, {  1.5f,  2.5f }, { -2.5f,  2.5f } };
            Assert::IsFalse(Polygon(rect).contains({ -2.6f, 0.0f }));
        }
        TEST_METHOD(Contains_NegativeFractionalTriangle_Inside)
        {
            std::vector<Point> tri{ { -3.2f, -1.1f }, {  0.0f, -4.3f }, {  2.7f, -1.2f } };
            Assert::IsTrue(Polygon(tri).contains({ -1.0f, -2.0f }));
        }
        TEST_METHOD(Contains_NegativeFractionalTriangle_Outside)
        {
            std::vector<Point> tri{ { -3.2f, -1.1f }, {  0.0f, -4.3f }, {  2.7f, -1.2f } };
            Assert::IsFalse(Polygon(tri).contains({ 0.0f, -5.0f }));
        }
        TEST_METHOD(Contains_Concave_NegativeFractional_Cavity)
        {
            std::vector<Point> uNegFrac{
                { -0.5f, -0.5f },
                { -4.5f, -0.5f },
                { -4.5f, -4.5f },
                { -3.5f, -4.5f },
                { -3.5f, -1.5f },
                { -1.5f, -1.5f },
                { -1.5f, -4.5f },
                { -0.5f, -4.5f }
            };
            // Точка (-2.5, -2.5) в полости «U», должна быть вне
            Assert::IsFalse(Polygon(uNegFrac).contains({ -2.5f, -2.5f }));
        }
    };
}
