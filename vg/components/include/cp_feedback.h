#pragma once
#include "cpmaterial.h"
#include "cpprimitive_object.h"
struct cpfeedback
{
     ps_mtl _pmtl;
     ps_primrive_object _pprm;
     GLuint _gpuOutputBuffer;
#if defind(WITHIN_IDE)
	bool _sel{ false };
#endif
	cpfeedback(ps_mtl& pmtl, ps_primrive_object& pprm);
     ~cp_feedback();
     bool get_output_vertex(vector<float>& overtex);
     void draw();
};
struct feedback_key
{
	string _mtl_key;
	string _prm_key;
	bool operator <(const feedback_key& tkey) const
	{
		if (_mtl_key!=tkey._mtl_key)
		{
			return _mtl_key < tkey._mtl_key;
		}
		else
		{
			return _prm_key < tkey._prm_key;
		}
	}
};
using psfeed_back = shared_ptr<cp_feedback>;
using mp_feed_back = map<feedback_key, psfeed_back>;
extern mp_feed_back g_feedback_list;
