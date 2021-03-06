/*  This file is part of distributed Structured Prediction (dSP) - http://www.alexander-schwing.de/
 *
 *  distributed Structured Prediction (dSP) is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  distributed Structured Prediction (dSP) is distributed in the hope
 *  that it will be useful, but WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE. See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with distributed Structured Prediction (dSP).
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (C) 2010-2013  Alexander G. Schwing  [http://www.alexander-schwing.de/]
 */

//Author: Alexander G. Schwing

/*  ON A PERSONAL NOTE: I spent a significant amount of time to go through
 *  both, theoretical justifications and coding of this framework.
 *  I hope the package is useful for your task. Any requests, 
 *  feedback, donations and support would be greatly appreciated.
 *  Thank you for contacting me!
 */
#ifndef __LIB__DHSAMPPMULTM__
#define __LIB__DHSAMPPMULTM__

#include <vector>
#include <string>

#include "libDHBaseMultM.h"
#include "../libRegionBPP/libRegionBPP.h"

template <class T, class S>
class libDHSampPMultM : public libDHBaseMultM<T> {
	template <class Y> struct Sample {
		std::vector<Y>* x;
	};
public:
	typedef typename std::vector<typename S::MPNode*> GRAPH_TYPE;
private:
	struct libDHBaseMultM<T>::SPParameters* DHParams;
	size_t NumFeatures;

	int ClusterSize;
	int Rank;
	bool isMyGraph;
	bool isMyObs;

	struct Sample<GRAPH_TYPE>* phi;
	std::vector<std::vector<size_t> >* y;
	std::vector<S*> rBP;
	std::vector<T> ObservedEmpiricalMean;

	int ReadRegionFile(const char* fn, std::vector<typename S::MPNode*>& Graph, bool WithFeatureID, T (*conversion)(T) = NULL);
	int ReadRegionFileBinary(const char* fn, std::vector<typename S::MPNode*>& Graph, bool WithFeatureID, T (*conversion)(T) = NULL);
	std::string ReplaceExtension(std::string orig, std::string extension);
	int ReadObservationFile(const char* fn, std::vector<size_t>& y);

	void GetAncestors(std::set<typename S::MPNode*>& Ancestors, typename S::MPNode* r);

	int ClearFeature(std::vector<typename S::MPNode*>& Graph);
	int Marginalize(std::vector<GRAPH_TYPE>* graph);
	T ComputeFandGSpecial(std::vector<T> *theta, std::vector<T> *grad, int ToReturn, size_t MPIterations);
	int ExchangeNu();
	int ExchangeNuPerSample();
	int MergeBeliefsToRank0(std::vector<std::map<size_t,std::vector<T> > >& Beliefs);
public:
	int DHClear();
	int ComputeEmpiricalMean(std::vector<T>* empMean);
	T ComputeEmpiricalMean(std::vector<T>* empMean, std::vector<T>* theta);
	int ClearMessages();
	T ComputeFandG(std::vector<T> *theta, std::vector<T> *empiricalMeans, std::vector<T> *grad, int flag);
public:
	libDHSampPMultM(struct libDHBaseMultM<T>::SPParameters* params);
	libDHSampPMultM(struct libDHBaseMultM<T>::SPParameters* params, std::vector<GRAPH_TYPE>* graph, std::vector<std::vector<size_t> >* obs, bool marginalize = false);
	~libDHSampPMultM();

	size_t GetNumFeatures();
	int Predict(std::vector<T> *theta, std::vector<std::map<size_t,std::vector<T> > >& Beliefs);
	int GetClusterSize();
	int GetRank();
	std::vector<typename libDHSampPMultM<T,S>::GRAPH_TYPE>* GetGraph();
	std::vector<std::vector<size_t> >* GetObservation();
};

#endif