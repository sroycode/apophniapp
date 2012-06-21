#include "Default.hh"
#include "Work.hpp"
#include "ImageFile.hpp"
#include <iostream>

/**
 * Constructor : private Constructor
 *
	 * @param progname
	 *   String executable name
	 *
 * @param referdir
 *   String reference directory
 *
 * @param cachedir
 *   String cache directory
 *
 * @return
 *   none
 */
apnr::Work::Work (std::string progname, std::string referdir , std::string cachedir) :
	referdir_(referdir),
	cachedir_(cachedir)
{
	ImageFile::init(progname);
}
/**
 * Work::run: mandatory function for web interface
 *
 * @param W
 *   WebObject shared_ptr W
 *
 * @return
 *   Bool status
 */
bool apnr::Work::run(apn::WebObject::pointer W)
{
	bool status=false;
	try {
		ImageFile Im=ImageFile(referdir_,cachedir_,W);
		return Im.WriteImage();
	} catch (apn::GenericException& e) {
		std::cerr << e.ErrorMsg_ << e.ErrorFor_ << std::endl;
	} catch (...) {
		std::cerr << "Unknown Runtime Error" << std::endl;
	}
	return status;
}
