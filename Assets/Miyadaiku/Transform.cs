using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiyadaikuEngine
{
    // TODO this should be Hierarchical
    public class Transform
    {
        public Vector3 Positon { get { return LocalPositon; } set { LocalPositon = value; } }
        public Vector3 LocalPositon { get; set; } = new Vector3();

        public Quaternion Rotation { get { return LocalRotation; } set { LocalRotation = value; } }
        public Quaternion LocalRotation { get; set; } = Quaternion.Identity;

        public Vector3 eulerAngles
        {
            get { return Rotation.ToEulerAngles(); }
            set { Rotation = Quaternion.CreateFromEulerAngles(value); }
        }
        public Vector3 LocalEulerAngles
        {
            get { return LocalRotation.ToEulerAngles(); }
            set { LocalRotation = Quaternion.CreateFromEulerAngles(value); }
        }

        public Vector3 Scale { get { return LocalScale; } set { LocalScale = value; } }
        public Vector3 LocalScale { get; set; } = new Vector3();

        public Matrix WorldMatrix
        {
            get 
            {
                Matrix m = Matrix.Identity;
                m *= Matrix.CreateScale(Scale);
                m *= Matrix.CreateFromQuaternion(Rotation);
                m *= Matrix.CreateTranslation(Positon);
                return m;
            }
            set { WorldMatrix = value; }
        }
    }
}
