#ifndef FRY_SCRIPT_H
#define FRY_SCRIPT_H

namespace fry_script
{
	/**
	 * Initializes the scripting-engine
	 */
	void Init();

	/**
	 * Tear down the scripting-engine and releases all resources
	 */
	void Shutdown();
}

#endif