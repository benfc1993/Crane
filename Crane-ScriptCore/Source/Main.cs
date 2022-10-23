using System;
using System.Runtime.CompilerServices;

namespace Crane {

	public struct Vector3
	{
		public float x, y, z;
		
		public Vector3(float x, float y, float z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
	}
	public class Main
	{

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern static string Print(string msg);

		public float FloatVar { get; set; }

		public Main()
		{
		}

		public int PrintMessage()
		{
			return 5 + 6 * 10;
		}

		public int PrintInt(ref int value)
		{
			Print("Hello, World!");
			return 2 * value;
		}

		public void PrintInts(int value1, int value2)
		{
			Print($"C# says: {value1} and {value2}");
		}

		public void PrintCustomMessage(string message)
		{
			Print($"C# says: {message}");
		}

	}
}
