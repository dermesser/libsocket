# ifndef _EXCEPTION_H
# define _EXCEPTION_H

# include <string>

using std::string;
namespace libsocket
{
    /**
     * @brief This class is instantiated and thrown when an error occurs.
     */
    struct socket_exception
    {
	int err; //!< This is the value of errno when the error occurred.
	string mesg; //!< This is the message, showing file, line and a description.

	socket_exception(string,int,string);
    };
}
# endif
