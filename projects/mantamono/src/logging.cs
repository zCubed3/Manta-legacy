using System.Runtime.CompilerServices;

using MantaInternal;

namespace MantaInternal {
   public static partial class CMethodBindings {
      [MethodImplAttribute(MethodImplOptions.InternalCall)]
      public static extern void __c__printf(string data);
   }
}

namespace Manta {

   public static partial class Debug {
      public static void Log(string message, bool newline=true) {
	 CMethodBindings.__c__printf($"{message}{(newline ? "\n" : "")}"); 
      }
   }

}
