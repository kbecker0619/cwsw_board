/** @file
 *	@brief	API common to all instances of the cwsw_board reusable component.
 *
 *	\copyright
 *	Copyright (c) 2020 Kevin L. Becker. All rights reserved.
 *
 *	Original:
 *	Created on: Feb 14, 2018
 *	Author: KBECKE35
 */

#ifndef CWSW_BOARD_CWSW_BOARD_COMMON_H_
#define CWSW_BOARD_CWSW_BOARD_COMMON_H_

#ifdef	__cplusplus
extern "C" {
#endif

// ============================================================================
// ----	Include Files ---------------------------------------------------------
// ============================================================================

#include "projcfg.h"

// ----	System Headers --------------------------
#include <stdbool.h>

// ----	Project Headers -------------------------

// ----	Module Headers --------------------------


// ============================================================================
// ----	Constants -------------------------------------------------------------
// ============================================================================

enum eErrorCodes_Board {
	kErr_Bsp_NoError = kErr_Lib_NoError,
	kErr_Bsp_NotInitialized,
	kErr_Bsp_BadParm,
	kErr_Bsp_InitFailed
};


// ============================================================================
// ----	Type Definitions ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public Variables ------------------------------------------------------
// ============================================================================

// ============================================================================
// ----	Public API ------------------------------------------------------------
// ============================================================================

// ==== Discrete Functions ================================================== {
/** Initialization for the Board Support component.
 * 	Target for `Init(Cwsw_Board)` API.
 *
 *	This function's responsibility is to set up the local vars, and manage the
 *	necessary HW, to prepare for the task function's 1st call (once the
 *	scheduler has been started).
 *
 * This function "connects" the board to the CPU.
 * It knows nothing about the application.
 *
 *	This function shall be called before the main scheduler is started.
 *
 *	As general guidance, the CWSW Library component should have been initialized
 *	before this function is called (since this module depends on the library),
 *	though this is not a strict requirement.
 *
 *	As general guidance, this should be after architecture initialization but
 *	before application initialization functions.
 *
 *	In the dependency tree, this module sits at the "between" the architecture
 *	and services and application, excepting for calls into support libraries.
 *
 *	@returns error code, or 0 for no problem (i.e., success).
 */
extern uint16_t Cwsw_Board__Init(void);

/** Target for Get(Cwsw_Board, Initialized) interface */
extern bool 	Cwsw_Board__Get_Initialized(void);


// ==== /Discrete Functions ================================================= }

// ==== Targets for Get/Set APIs ============================================ {
/** "Chapter Designator" for Get/Set API.
 *	Intentionally unused symbol, designed to get you to the correct starting
 *	point, when you want to find macros for the Get/Set API; simply highlight
 *	the Module argument in your IDE (e.g, Eclipse, NetBeans, etc.), and select
 *	Go To Definition.
 */
enum { Cwsw_Board = 2 };	/* Component ID for boards; all supported boards have the same ID */


/** Target symbol for Get(Cwsw_Board, xxx) interface */
#define Cwsw_Board__Get(resource)			Cwsw_Board__Get_ ## resource()


/** Target symbol for Set(Cwsw_Board, Resource, xxx) interface */
#define Cwsw_Board__Set(resource, value)	Cwsw_Board__Set_ ## resource(value)

// ==== /Targets for Get/Set APIs =========================================== }

#ifdef	__cplusplus
}
#endif

// ====	COMPONENT DOCUMENTATION ============================================= {
#if defined(IN_DOXY)				/* { */
/* klb: i apologize in advance for burdening your build, by forcing your compiler to consume all of
 * this information every build, every file that includes it. My only justification is that this is
 * the only place common to all boards, and, I want to limit reliance on supplemental
 * documents because those get lost. At least if the documentation is in the code, there is no
 * chance it'll get lost.
 */

/**	\page Board_Head Reusable Component : Board Support
 *	@tableofcontents
 * 		@subpage Board_Introduction \n
 * 		@subpage Board_Design \n
 * 		@subpage Board_Integration \n
 * 		@subpage Board_srs \n
 */

/**	\page Board_Introduction					Introduction to the Board reusable component
 *	@tableofcontents
 *	This chapter briefly describes the design and implementation of the Board
 *	reusable component member of the CWSW library.
 *
 *	This chapter also briefly describes the integration of this feature into the
 *	larger code base of the user software project.
 *
 *	The intended audience of this document is any software engineer who uses the
 *	CWSW library and wants to add support for one (or more) of the supported
 *	architectures.
 *
 *	@section Board_directory_layout				Directory Layout
 *	The Includes structure here is slightly different than in other CWSW
 *	components designed for reuse / multiple deployments; it is similar to the
 *	layout of the [Arch component](@ref Arch_Introduction).
 *
 *	In most CWSW components, the main API is published at the root of the
 *	component folder; any local-to-component headers are located within
 *	appropriate subfolders.
 *
 *	For this component, the main API is at the root of the board-specific folder.
 *	While there is an API file that exposes all APIs common to all boards
 *	located at the component root, this is not intended to be used by projects
 *	that use the Board-Support component.
 *
 *
 *	@section Board_related_docs					Applicable Documents
 *	TBD
 *
 *	@section Board_terms						Terms, Acronyms, Abbreviations
 *	TBD
 */

/**	\page Board_Design							Board Subsystem Design
 *	@tableofcontents
 *		@subpage Board_Responsibilities \n
 *		@subpage Board_Constraints \n
 *		@subpage Board_Interactions \n
 *		@subpage Board_Composition \n
 *		@subpage Board_Resources \n
 *		@subpage Board_Interfaces \n
 *		@subpage Board_Main_Obj_Diag \n
 *		@subpage Board_Dsm \n
 */

/**	@page Board_Responsibilities				Component Responsibilities
 *	@tableofcontents
 * 	The Board component manages all aspects of the board.
 * 	In some cases, one Component may be configured to handle multiple
 * 	variants of the board; however, in most cases, there will be one Board
 * 	component per board variant.
 *
 * 	It is assumed that each board is built to be driven by one MCU, so we
 * 	do not at this time handle the use case where there is an
 * 	interchangeable "MCU personality" board to allow multiple MCUs.
 */

/**	@page Board_Constraints						Constraints Imposed Upon Component
 *	@tableofcontents
 *	-	TBD. Component resides "above" the Architecture in the dependency
 *		tree, and provides services and functions to services and application
 *		components.
 */

/**	\page Board_Resources      			  	   	Resources
 *	@tableofcontents
 *	TBD
 */

/**	@page Board_Integration						Integrating the Reusable Component
 *	@tableofcontents
 *	To add the facilities of the CWSW Board component to your project, follow these steps.
 *
 *	@Note Author's note: These steps are generic for all IDEs; however, the
 *	specific details (e.g., screen shots) are oriented toward IDEs based on
 *	Eclipse, such as NXP S32DS; ARM DS-5; or TI's Code Composer.
 *	While I have implemented these steps in Microchip's MPLAB v4.x, the steps
 *	are so close that I didn't feel IDE-specific instructions were required.
 *
 *	Integration Steps:
 *	-# [Share the Source](#bd_source_share)
 *	-# [Add Dependencies](#bd_add_depends)
 *	-# [Update Includes paths](#bd_update_includes)
 *	-# [Configuration](#bd_comp_configuration)
 *	-# [Update Initialization Code](#bd_init_code)
 *	-# Optional APIs.
 */

	/**	@page Board_Integration
	 *	@section bd_source_share				Share the Source
	 *	Designed to be a reusable component, the CWSW Board component is
	 *	organized into a reusable section, and requires additional project-
	 *	specific configuration items in the project.
	 *
	 *	The component layout in your workspace looks like this:
	 *	@code{.sh}

		cwsw_board_prj/
		├── cwsw_board
		│   ├── none
		│   └── pic32mz_efm_sk
		├── doc
		│   └── images
		└── ut

	 *	@endcode
	 *	@note This component is presently under construction; as support is added
	 *	for additional project boards, the exact form of the directory tree
	 *	shown above may change.
	 *	@note Some features have been omitted for clarity.
	 *
	 *	The <code>cwsw_board_prj</code> directory is the home to the various
	 *	project-management files (such as the Doxygen configuration, the Eclipse
	 *	project files, etc.), and is the parent for the component.
	 *
	 *	The <code>ut</code> directory is the source for the developer workspace
	 *	for this reusable component, and illustrates how to configure the
	 *	component is client projects.
	 *
	 *	The <code>doc</code> directory holds relevant design information not
	 *	included in this Doxygen format.
	 *
	 *	The <code>cwsw_board</code> directory is the sharable and reusable portion
	 *	of the component.\n
	 *	The design intent is for this folder to be shared into your project's
	 *	directory structure.
	 *	-	@b Preferred: If your code management system can share folders
	 *		(e.g., PVCS / Dimensions, MKS), this might be a good solution; else,
	 *	-	@b Symlinks: filesystem symlinks are supported by GIT, with
	 *		appropriate configuration options set properly.
	 *	-	Example (using symlinks):\n
	 *	Given the following workspace directory structure:
	 *	@code{.sh}

	 	(workspace)
		├── components
		│   ├── cwsw_arch_prj
		│   ├── cwsw_board_prj
		│   └── cwsw_lib_prj
		└── cwsw_example_prj
		    ├── appl_swc
		    ├── cfg
		    ├── drivers
		    ├── libs
		    ├── os
		    ├── rte
		    └── services

	 *	@endcode
	 *	Execute the following. @note This example is given using Cygwin; the same
	 *	command line verbatim would be done in Git Bash.\n
	 *	Also, 7-Zip can create symlinks, and there are other methods in Windows to create symbolic
	 *	links.
	 *
	 *	@code{.sh}

		$ cd cwsw_example_proj/drivers/
		$ ln -s ../../components/cwsw_board_prj/cwsw_arc cwsw_board
		$ git add cwsw_board

		$ tree ../.. -dL 3
		(workspace)
		├── components
		│   ├── cwsw_arch_prj
		│   ├── cwsw_board_prj
		│   │   └── cwsw_board
		│   └── cwsw_lib_prj
		└── cwsw_example_prj
		    ├── appl_swc
		    ├── cfg
		    ├── drivers
		    │   └── cwsw_board -> ../../components/cwsw_board_prj/cwsw_board
		    ├── libs
		    ├── os
		    ├── rte
		    └── services

	 *	@endcode
	 *	@note The placement above, in the <code> drivers </code> folder, is
	 *	only a suggestion. Wherever you locate it, if you are using filesystem
	 *	symlinks, it is important to use relative symlinks to maintain
	 *	portability from one dev system to another. <em>(If you are using a
	 *	sharing mechanism from your source versioning system, use the most
	 *	appropriate linking or referencing mechanism is appropriate.)</em>
	 *	\n \n
	 */

	/**	@page Board_Integration
	 *	@section bd_add_depends					Add Component Dependencies
	 *
	 *	The <code>cwsw_board</code> component has the following dependencies
	 *	to other cwsw components:
	 *	-	<code>cwsw_lib</code>. To integrate this component, refer to the
	 *		integration instructions [here](@ref Lib_Integration).
	 *	-	<code>cwsw_arch</code>. To integrate this component, refer to the
	 *		integration instructions [here](@ref Arch_Integration).
	 *
	 *	Here is one example of how to do this, using the Cygwin command line:
	 *	@code

		$ cd cwsw_example_proj/drivers
		$ ln -s ../../components/cwsw_arch_prj/cwsw_arch cwsw_arch
		$ cd ../libs/
		$ ln -s ../../components/cwsw_lib_prj/cwsw_lib cwsw_lib

		$ tree ../.. -dL 3
		(workspace)
		├── components
		│   ├── cwsw_arch_prj
		│   │   └── cwsw_arch
		│   ├── cwsw_board_prj
		│   │   └── cwsw_board
		│   └── cwsw_lib_prj
		│       └── cwsw_lib
		└── cwsw_example_proj
		    ├── appl_swc
		    ├── cfg
		    ├── drivers
		    │   ├── cwsw_arch -> ../../components/cwsw_arch_prj/cwsw_arch
		    │   └── cwsw_board -> ../../components/cwsw_board_prj/cwsw_board
		    ├── libs
		    │   └── cwsw_lib -> ../../components/cwsw_lib_prj/cwsw_lib
		    ├── os
		    ├── rte
		    └── services

	 *	@endcode
	 *
	 *	\n \n
	 */

	/**	@page Board_Integration
	 *	@section bd_update_includes				Update Includes paths
	 *	@subsection bd_update_includes_paths	Include Paths
	 *	Update your build system to add the relative paths to the selected
	 *	architecture folder in the <code>cwsw_board</code> folder. @b Note this
	 *	is different than in most other CWSW components; this path is one level
	 *	"deeper."
	 *
	 *	To clarify what you're looking for, here is the annotation for each of
	 *	the files in the <code> arch </code> project.
	 *	@note The design intent, is for all references within the project, to be
	 *	relative to the project itself. Portability is greatly reduced when the
	 *	project refers to fully qualified pathnames, or those outside of the
	 *	project tree itself.
	 *
	 *	@code{.sh}

		$ tree cwsw_board_prj/ -dL 2
		cwsw_board_prj/
		├── cwsw_board			<<== target for symlink into your project
		│   ├── none				<<== point your includes path to one and only one of the board ...
		│   └── pic32mz_efm_sk		<<== folders, and exclude the rest within your build section
		├── doc
		└── ut

	 *	@endcode
	 *	@note Some elements of the tree above have been excluded for clarity.
	 *
	 *	To illustrate how this might be done in an Eclipse-based project, see
	 *	the following screen capture: \n
	 *	@image html doc/images/eclipse_add_includes_path.png			"Example Includes Path"
	 *	\n
	 *
	 *	@subsection bd_exclude_undesireables		Exclude Undesired Boards
	 *	Because (at least a portion of) the API that faces "upwards" must be
	 *	the same across all supported architectures, multiple definitions of
	 *	those API functions will exist unless those modules are excluded from
	 *	your build.
	 *
	 *	In Eclipse and IDEs derived from it, the easiest way to do this is to
	 *	specifically exclude a folder from the Project Properties dialog. Here
	 *	is an example: \n
	 *	@image html doc/images/eclipse_exclude_folder_from_build.png	"Exclude Directory Item from Build"
	 *	This same "exclude" checkbox appears in multiple places in the project
	 *	properties dialog.
	 *
	 *	In makefile-based projects, simply omit reference to the undesired
	 *	board folders.
	 *	\n \n
	 */

	/**	@page Board_Integration
	 *	@section bd_comp_configuration			Configure the Reusable Component and its dependencies
	 *	In some or the supported architectures, no configuration beyond what
	 *	has already been described is necessary. In some others, additional
	 *	items may need to be configured.
	 *
	 *	If the configurations are configured via preprocessor defines, there are two
	 *	easy methods of accomplishing this:
	 *	+ command-line defines
	 *	+ preprocessor defines in the file "projcfg.h"
	 */

	/**	@page Board_Integration					Integrating the Reusable Component
	 *	@section bd_init_code					Update application's initialization code
	 *	At the time of this writing, none of the supported boards require any
	 *	secondary initialization (e.g., initialization calls made after the
	 *	scheduler is started).
	 *
	 *	While not strictly required, it is recommended that the CWSW Library is
	 *	initialized first; following this, the Architecture component can be
	 *	initialized, followed by the Board component.
	 *
	 *	One of the APIs introduced is an abstraction intended to give the same signature to all
	 *	CWSW modules: <code>Init(Component)</code>. This is optional, but when used, looks like
	 *	this:
	 *	@code{.c}

	 	void main(void)
	 	{
	 		(void)Init(Cwsw_Lib);
	 		(void)Init(Cwsw_Arch);
	 		(void)Init(Cwsw_Board);
	 		... more initialization

	 		... main code follows ...
	 	}

	 *	@endcode
	 *	\n \n
	 */


/**	\page Board_srs							Board Software Requirements
 *	@tableofcontents
 *	TBD. Might be delineated in an external document.
 */

#endif								/* } */
// ==== /COMPONENT DOCUMENTATION ============================================ }

#endif /* CWSW_BOARD_CWSW_BOARD_COMMON_H_ */
