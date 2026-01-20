#include "../../src/core/Cone.hpp"
#include "../../src/core/Point3D.hpp"
#include "../../src/core/Ray.hpp"
#include "../../src/core/Vector3D.hpp"
#include <cmath>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

using namespace Raytracer;
using namespace Math;

// Test fixture setup
static void setup(void)
{
    // This function is called before each test
}

static void teardown(void)
{
    // This function is called after each test
}

TestSuite(ConeTest, .init = setup, .fini = teardown);

// Test ray intersection with cone surface
Test(ConeTest, RayIntersectsSurface)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Ray ray(Point3D(1, -2, 0), Vector3D(0, 1, 0).normalize());

    double t = cone.hits(ray);
    cr_assert_gt(t, 0, "Ray should intersect cone surface");
}

// Test ray intersection with cone base
Test(ConeTest, RayIntersectsBase)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Ray ray(Point3D(1, -1, 0), Vector3D(0, 1, 0).normalize());

    double t = cone.hits(ray);
    cr_assert_gt(t, 0, "Ray should intersect cone base");
}

// Test ray missing the cone
Test(ConeTest, RayMissesCone)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Ray ray(Point3D(5, 0, 5), Vector3D(1, 0, 0).normalize());

    double t = cone.hits(ray);
    cr_assert_eq(t, -1.0, "Ray should miss the cone");
}

// Test normal vector on cone surface
Test(ConeTest, NormalOnSurface)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Point3D point(1, 2, 0); // Point on cone surface

    Vector3D normal = cone.getNormal(point);
    cr_assert_float_eq(normal.length(), 1.0, 1e-6, "Normal vector should be normalized");

    // The normal should make the correct angle with the axis
    // For a cone, if theta is the angle between axis and side (atan(r/h))
    // then the normal should be at angle (pi/2 - theta) with the axis
    double theta = std::atan2(cone.radius, cone.height);
    double expectedAngle = M_PI / 2.0 - theta;
    double angleWithAxis = std::acos(std::abs(normal.dot(Vector3D(0, 1, 0))));
    cr_assert_float_eq(angleWithAxis, expectedAngle, 1e-6, "Normal should have correct angle with axis");
}

Test(ConeTest, DisableCmdLineArgs, .init = NULL)
{
    // This test exists just to disable command line argument requirement
    cr_assert(true);
}

// Test normal vector on cone base
Test(ConeTest, NormalOnBase)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Point3D point(1, 0, 0); // Point on base

    Vector3D normal = cone.getNormal(point);
    Vector3D expectedNormal = Vector3D(0, -1, 0);

    cr_assert_float_eq(normal.x, expectedNormal.x, 1e-6, "Normal X component incorrect");
    cr_assert_float_eq(normal.y, expectedNormal.y, 1e-6, "Normal Y component incorrect");
    cr_assert_float_eq(normal.z, expectedNormal.z, 1e-6, "Normal Z component incorrect");
}

// Test edge case: normal at apex
Test(ConeTest, NormalAtApex)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Point3D apex(0, 4, 0);

    Vector3D normal = cone.getNormal(apex);
    cr_assert_float_eq(normal.length(), 1.0, 1e-6, "Normal vector should be normalized");
}

// Test edge case: point on cone axis
Test(ConeTest, NormalOnAxis)
{
    Cone cone(Point3D(0, 0, 0), 2, 4, Vector3D(0, 1, 0));
    Point3D point(0, 2, 0); // Point on axis

    Vector3D normal = cone.getNormal(point);
    cr_assert_float_eq(normal.length(), 1.0, 1e-6, "Normal vector should be normalized");

    // Normal should be perpendicular to the axis
    double dotProduct = std::abs(normal.dot(Vector3D(0, 1, 0)));
    cr_assert_float_eq(dotProduct, 0.0, 1e-6, "Normal should be perpendicular to axis");
}
