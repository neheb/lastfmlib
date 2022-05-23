//    Copyright (C) 2009 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef SUBMISSION_INFO_COLLECTION_H
#define SUBMISSION_INFO_COLLECTION_H

#include "submissioninfo.h"
#include <deque>
#include <string>

class SubmissionInfoCollection {
public:
    void addInfo(const SubmissionInfo& info);
    void clear();
    std::string getPostData() const;

private:
    std::deque<SubmissionInfo> m_Infos;
};

#endif
