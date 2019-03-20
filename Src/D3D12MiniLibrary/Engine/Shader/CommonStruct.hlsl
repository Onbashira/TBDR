

struct Camera
{
    matrix View;
    matrix Proj;
    float windowWidth;
    float windowHeight;
};

struct ShadowMapLight
{
    matrix view;
};

struct LightingLight
{
    matrix view;
    int type; // 0= Direction,1 = point,2 = spot,3=Hemi
};

