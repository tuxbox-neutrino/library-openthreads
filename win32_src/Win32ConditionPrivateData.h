//

// OpenThread library, Copyright (C) 2002 - 2003  The Open Thread Group

//

// This library is free software; you can redistribute it and/or

// modify it under the terms of the GNU Lesser General Public

// License as published by the Free Software Foundation; either

// version 2.1 of the License, or (at your option) any later version.

//

// This library is distributed in the hope that it will be useful,

// but WITHOUT ANY WARRANTY; without even the implied warranty of

// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU

// Lesser General Public License for more details.

// 

// You should have received a copy of the GNU Lesser General Public

// License along with this library; if not, write to the Free Software

// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

//



//

// WIN32ConditionPrivateData.h - Private data structure for Condition

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//



#ifndef _WIN32CONDITIONPRIVATEDATA_H_

#define _WIN32CONDITIONPRIVATEDATA_H_





#ifndef _WINDOWS_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#endif



#include <OpenThreads/Mutex>

#include <OpenThreads/Condition>



namespace OpenThreads {



class Win32ConditionPrivateData {

public:

	friend class Condition;



	/// number of waiters.

	long waiters_;



	Win32ConditionPrivateData ()

	{

		waiters_ = 0;

		sema_ = CreateSemaphore(NULL,0,0x7fffffff,NULL);

		waiters_done_ = CreateEvent(NULL,FALSE,FALSE,NULL);

	}



	~Win32ConditionPrivateData ();



	inline int broadcast ()

	{

		 waiters_lock_.lock();

		 int have_waiters = 0;



		if (waiters_ > 0)

		{

	      // We are broadcasting, even if there is just one waiter...

		  // Record the fact that we are broadcasting.  This helps the

		  // wait() method know how to optimize itself.  Be sure to

		  // set this with the <waiters_lock_> held.

	      was_broadcast_ = 1;

		  have_waiters = 1;

		}

		waiters_lock_.unlock();

		

		int result = 0;

		if (have_waiters)

	    {

			// Wake up all the waiters.

			ReleaseSemaphore(sema_,waiters_,NULL);

			WaitForSingleObject(waiters_done_,INFINITE) ;

			// This is okay, even without the <waiters_lock_> held because

			// no other waiter threads can wake up to access it.

			was_broadcast_ = 0;

	    }

		return result;

	}



	inline int signal()

	{

		 waiters_lock_.lock();

	     int have_waiters = waiters_ > 0;

		 waiters_lock_.unlock();

		

		int result = 0;

		if (have_waiters)

	    {

			if( !ReleaseSemaphore(sema_,1,NULL) )

				result = -1;

	    }

		return result;

	}



	inline int wait (Mutex& external_mutex, long timeout_ms)

	{

		// Prevent race conditions on the <waiters_> count.

		waiters_lock_.lock();

		waiters_++;

		waiters_lock_.unlock();

		

		int result = 0;

		

        external_mutex.unlock();

		DWORD dwResult = WaitForSingleObject(sema_,timeout_ms);

		if(dwResult != WAIT_OBJECT_0)

			result = (int)dwResult;

		

		// Reacquire lock to avoid race conditions on the <waiters_> count.

		waiters_lock_.lock();

		

		// We're ready to return, so there's one less waiter.

		waiters_--;

		

		int last_waiter = was_broadcast_ && waiters_ == 0;

		

		// Release the lock so that other collaborating threads can make

		// progress.

		waiters_lock_.unlock();

		

		if (result != -1 && last_waiter)

			SetEvent(waiters_done_);

		

		external_mutex.lock();

		

		return result;

	}





protected:



  /// Serialize access to the waiters count.

  Mutex waiters_lock_;



  /// Queue up threads waiting for the condition to become signaled.

  HANDLE sema_;

  /**

   * An auto reset event used by the broadcast/signal thread to wait

   * for the waiting thread(s) to wake up and get a chance at the

   * semaphore.

   */

  HANDLE waiters_done_;



  /// Keeps track of whether we were broadcasting or just signaling.

  size_t was_broadcast_;



};

}



#endif // !_WIN32CONDITIONPRIVATEDATA_H_

