#ifndef _APNR_IMAGEFILE_HPP_
#define _APNR_IMAGEFILE_HPP_
#define APNR_IMAGEFILE_PROGNO 2003

#include "Default.hh"
#include "MimeTypes.hpp"
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <Magick++.h>

#include <apn/Exception.hh>
#include <apn/WebObject.hpp>
#include <apn/MemoryMap.hpp>


class ImageFile {
public:
	enum ImageStatus { NO_IDEA=0, EXACT_IN_CACHE, EXACT_IN_REFER,
	                   BASE_IN_CACHE, BASE_IN_REFER, FOREIGN_IN_CACHE,
	                   FOREIGN_IN_REFER, BASE_NOT_FOUND, BAD_IMAGE
	                 };

	/**
	 * init : static initalize
	 *
	 * @param progname
	 *   String this executable name
	 *
	 * @return
	 *   none
	 */
	static void init (std::string progname) {
		Magick::InitializeMagick(progname.c_str());
	}
	/**
	 * Constructor : private Constructor
	 *
	 * @param referdir
	 *   String reference directory
	 *
	 * @param cachedir
	 *   String cache directory
	 *
	 * @param W
	 *   WebObject shared_ptr W
	 *
	 *
	 * @return
	 *   none
	 */
	ImageFile(const std::string referdir, const std::string cachedir, apn::WebObject::pointer W) :
		referdir_(referdir),
		cachedir_(cachedir),
		WopInstance(W),
		x(0),y(0),
		iStatus(NO_IDEA),
		noresize(false) {
		try {
			std::size_t ct = WopInstance->GetURLPartCount();
			if (ct<1)
				throw apn::GenericException(0,"invalid URL","");
			std::string file = WopInstance->GetURLPart(ct-1);
			std::string dir;
			for (std::size_t i=0; i<ct-1; ++i) {
				dir += APNR_DEFAULT_STRN_DIRSEP + WopInstance->GetURLPart(i);
			}
			std::string::size_type ldot= file.find_last_of(APNR_DEFAULT_STRN_DOT);
			if (ldot == std::string::npos)
				throw apn::GenericException(APNR_IMAGEFILE_PROGNO,"no extn","");
			extn=file.substr(ldot+1);
			std::string name = file.substr(0,ldot);
			std::string mime_type=apnr::mime_types::get_type(extn);
			WopInstance->SetContentType(mime_type);
			writepath = APNR_DEFAULT_STRN_DIRSEP + dir + APNR_DEFAULT_STRN_DIRSEP + file;
			readpath=writepath;
			if (FindFile(cachedir_ + readpath)) {
				iStatus = EXACT_IN_CACHE;
				throw apn::NullException();
			} else if (FindFile(referdir_ + readpath)) {
				iStatus = EXACT_IN_REFER;
				throw apn::NullException();
			} else {
				boost::xpressive::sregex rex = boost::xpressive::sregex::compile( "(\\S+)_r(\\d+)x(\\d+).(\\w+)$" );
				boost::xpressive::smatch what;
				if( regex_match( file, what, rex ) ) {
					name= std::string(what[1]);
					x= apn::Convert::AnyToAny<std::string,int>(what[2]);
					y= apn::Convert::AnyToAny<std::string,int>(what[3]);
				} else {
					noresize=true;
				}
				readpath = APNR_DEFAULT_STRN_DIRSEP + dir + APNR_DEFAULT_STRN_DIRSEP +
				           name + APNR_DEFAULT_STRN_DOT + extn;
				if (FindFile(cachedir_ + readpath)) {
					iStatus = BASE_IN_CACHE;
					throw apn::NullException();
				} else if (FindFile(referdir_ + readpath)) {
					iStatus = BASE_IN_REFER;
					throw apn::NullException();
				} else {
					typedef std::vector<std::string> sVec;
					sVec s = apnr::mime_types::get_all_extns<sVec>();
					for (sVec::const_iterator it = s.begin(); it != s.end() ; ++it ) {
						readpath = APNR_DEFAULT_STRN_DIRSEP + dir + APNR_DEFAULT_STRN_DIRSEP +
						           name + APNR_DEFAULT_STRN_DOT + *it;
						if (FindFile(cachedir_ + readpath)) {
							iStatus = FOREIGN_IN_CACHE;
							throw apn::NullException();
						} else if (FindFile(referdir_ + readpath)) {
							iStatus = FOREIGN_IN_REFER;
							throw apn::NullException();
						}
					}
				}
			}
		} catch (apn::NullException& e) {
		} catch (apn::GenericException& e) {
			iStatus=BAD_IMAGE;
		} catch (...) {
			iStatus=BAD_IMAGE;
		}
	}
	/**
	 * virtual destructor
	 */
	virtual ~ImageFile () {}
	/**
	 * WriteImage : write new image if required
	 *
	 * @return
	 *   Bool status
	 */
	bool WriteImage() {
		bool status=false;
		switch (iStatus) {
		case NO_IDEA:
		case BAD_IMAGE:
			break;
		case EXACT_IN_REFER:
			status=PopulateWeb(referdir_ + writepath);
			break;
		case EXACT_IN_CACHE:
			status=PopulateWeb(cachedir_ + writepath);
			break;
		case BASE_IN_REFER: {
			if (MakeImage(referdir_ + readpath,cachedir_ + writepath)) {
				status=PopulateWeb(cachedir_ + writepath);
			} else {
				status=false;
			}
		}
		break;
		case BASE_IN_CACHE: {
			if (MakeImage(cachedir_ + readpath,cachedir_ + writepath)) {
				status=PopulateWeb(cachedir_ + writepath);
			} else {
				status=false;
			}
		}
		break;
		case FOREIGN_IN_REFER: {
			if (MakeImage(referdir_ + readpath,cachedir_ + writepath)) {
				status=PopulateWeb(cachedir_ + writepath);
			} else {
				status=false;
			}
		}
		break;
		case FOREIGN_IN_CACHE: {
			if (MakeImage(cachedir_ + readpath,cachedir_ + writepath)) {
				status=PopulateWeb(cachedir_ + writepath);
			} else {
				status=false;
			}
		}
		break;
		default:
			break;
		}
		return status;
	}
private:
	const std::string referdir_;
	const std::string cachedir_;
	apn::WebObject::pointer WopInstance;
	std::string writepath;
	std::string readpath;
	std::string extn;
	int x;
	int y;
	ImageStatus iStatus;
	bool noresize;

	/**
	 * FindFile: Find a file
	 *
	 * @param name
	 *   String file to find
	 *
	 * @return
	 *   Bool status
	 */
	bool FindFile(const std::string name) {
		return boost::filesystem::exists(boost::filesystem::path(name.c_str()));
	}

	/**
	 * MoveOrDelFile: move a file , delete if already exists
	 *
	 * @param name
	 *   String file to move
	 *
	 * @param location
	 *   String final location
	 *
	 * @return
	 *   none
	 */
	bool MoveOrDelFile(const std::string name, const std::string location) {
		bool status=false;
		try {
			boost::filesystem::path from_p = boost::filesystem::path(name.c_str());
			boost::filesystem::path to_p = boost::filesystem::path(location.c_str());
			if ( (boost::filesystem::exists(from_p)) && (!boost::filesystem::exists(to_p)) ) {
				boost::filesystem::rename(from_p,to_p);
				status=true;
			} else if ( (boost::filesystem::exists(from_p)) && (boost::filesystem::exists(to_p)) ) {
				boost::filesystem::remove(from_p);
				status=true;
			}
		} catch (boost::filesystem::filesystem_error &e) {
			status=false;
		}
		return status;
	}
	/**
	 * MakeImage : populate the image
	 *
	 * @param readfrom
	 *   String location of file to read
	 *
	 * @param writeto
	 *   String location of file to write
	 *
	 * @return
	 *   Bool status
	 */
	bool MakeImage(const std::string readfrom, const std::string writeto) {
		std::string tmppath = cachedir_
		                      + boost::posix_time::to_iso_string(boost::posix_time::microsec_clock::local_time())
		                      + extn;
		std::string xy = apn::Convert::AnyToAny<int,std::string>(x)
		                 + APNR_DEFAULT_STRN_X + apn::Convert::AnyToAny<int,std::string>(y);
		Magick::Image image(readfrom);
		if (!noresize) {
			Magick::Geometry myGeo;
			myGeo.width(myGeo.width()+x);
			myGeo.height(myGeo.height()+y);
			myGeo.aspect(true);
			image.backgroundColor("white");
			image.scale(myGeo);
		}
		std::string image_type = apnr::mime_types::get_name(extn);
		image.magick(image_type);
		image.write(tmppath);
		return (MoveOrDelFile(tmppath,writeto));
	}
	/**
	 * PopulateWeb : populate Web Object
	 *
	 * @param location
	 *   String location of file to serve
	 *
	 * @return
	 *   Bool status
	 */
	bool PopulateWeb(const std::string location) {
		bool status=false;
		try {
			char buffer[APNR_DEFAULT_BUFF_SIZE+1];
			apn::MemoryMap::Len l;
			apn::MemoryMap m_file(location.c_str(), APNR_DEFAULT_PAGES_USED);

			while (m_file.residual()>0) {
				l = m_file.read_bytes(buffer, APNR_DEFAULT_BUFF_SIZE);
				WopInstance->AddResponse(buffer,(std::size_t)l);
			}
			status=true;
		} catch (...) {
			status=false;
		}
		return status;
	}
};
#endif /* _APNR_IMAGEFILE_HPP_ */
