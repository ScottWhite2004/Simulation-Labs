#include "Cuboid.h"

void Cuboid::create()
{
    const float hx = _width * 0.5f;
    const float hy = _height * 0.5f;
    const float hz = _depth * 0.5f;

    setVertices({
        // Face 1 (+X)
        {{ glm::vec3(hx, -hy, -hz)  }, {1,1,1}, {1,0}, { 1, 0, 0}},
        {{ glm::vec3(hx,  hy, -hz)  }, {1,1,1}, {1,1}, { 1, 0, 0}},
        {{ glm::vec3(hx,  hy,  hz)  }, {1,1,1}, {0,1}, { 1, 0, 0}},
        {{ glm::vec3(hx, -hy,  hz)  }, {1,1,1}, {0,0}, { 1, 0, 0}},

        // Face 2 (-X)
        {{ glm::vec3(-hx, -hy,  hz)  }, {1,1,1}, {1,0}, {-1, 0, 0}},
        {{ glm::vec3(-hx,  hy,  hz)  }, {1,1,1}, {1,1}, {-1, 0, 0}},
        {{ glm::vec3(-hx,  hy, -hz)  }, {1,1,1}, {0,1}, {-1, 0, 0}},
        {{ glm::vec3(-hx, -hy, -hz)  }, {1,1,1}, {0,0}, {-1, 0, 0}},

        // Face 3 (+Y)
        {{ glm::vec3(-hx,  hy, -hz)  }, {1,1,1}, {0,1}, { 0, 1, 0}},
        {{ glm::vec3(-hx,  hy,  hz)  }, {1,1,1}, {0,0}, { 0, 1, 0}},
        {{ glm::vec3(hx,  hy,  hz)  }, {1,1,1}, {1,0}, { 0, 1, 0}},
        {{ glm::vec3(hx,  hy, -hz)  }, {1,1,1}, {1,1}, { 0, 1, 0}},

        // Face 4 (-Y)
        {{ glm::vec3(-hx, -hy,  hz)  }, {1,1,1}, {0,0}, { 0,-1, 0}},
        {{ glm::vec3(-hx, -hy, -hz)  }, {1,1,1}, {0,1}, { 0,-1, 0}},
        {{ glm::vec3(hx, -hy, -hz)  }, {1,1,1}, {1,1}, { 0,-1, 0}},
        {{ glm::vec3(hx, -hy,  hz)  }, {1,1,1}, {1,0}, { 0,-1, 0}},

        // Face 5 (+Z)
        {{ glm::vec3(-hx, -hy,  hz)  }, {1,1,1}, {0,0}, { 0, 0, 1}},
        {{ glm::vec3(hx, -hy,  hz)  }, {1,1,1}, {1,0}, { 0, 0, 1}},
        {{ glm::vec3(hx,  hy,  hz)  }, {1,1,1}, {1,1}, { 0, 0, 1}},
        {{ glm::vec3(-hx,  hy,  hz)  }, {1,1,1}, {0,1}, { 0, 0, 1}},

        // Face 6 (-Z)
        {{ glm::vec3(hx, -hy, -hz)  }, {1,1,1}, {0,0}, { 0, 0,-1}},
        {{ glm::vec3(-hx, -hy, -hz)  }, {1,1,1}, {1,0}, { 0, 0,-1}},
        {{ glm::vec3(-hx,  hy, -hz)  }, {1,1,1}, {1,1}, { 0, 0,-1}},
        {{ glm::vec3(hx,  hy, -hz)  }, {1,1,1}, {0,1}, { 0, 0,-1}},
        });

    setIndices({
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9,10,10,11, 8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20
        });
}

void Cuboid::move()
{
}

Cuboid::~Cuboid() = default;
