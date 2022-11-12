using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wiwa
{
    public struct Transform
    {
        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale;

        public Transform(Vector3 pos, Vector3 rot, Vector3 scl)
        {
            Position = pos;
            Rotation = rot;
            Scale = scl;
        }
    }
}
