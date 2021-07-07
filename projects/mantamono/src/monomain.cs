using Manta;

namespace MantaMonoInternal {
   static class MantaTests {
      private static void __manta_heartbeat() {
	 while (true)
	    Debug.Log("Heartbeat");
      }
   }
}
