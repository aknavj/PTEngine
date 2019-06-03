#ifndef __CAMERA_TASK_H__
#define __CAMERA_TASK_H__

/*
 */
class CCameraTask : public CKernelTask, public TSingleton<CCameraTask> {

	public:
		DEFAULT_TASK_CREATOR(CCameraTask);

		virtual bool Start();

		virtual void Update();

		virtual void Stop();

		void Bounce();

		float GetThrust() const;
		const CSRTTransform &GetCamera() const;
		CVector GetVelocity() const;
		CVector GetPosition() const;

		void SetThrust(const float f);
		void SetVelocity(const CVector &v);
		void SetPosition(const CVector &v);

	protected:
		float m_fThrust;
		CVector m_vOldPos;
		CVector m_vVelocity;
		CSRTTransform m_srtCamera;	
};

#endif // __CameraTask_h__
