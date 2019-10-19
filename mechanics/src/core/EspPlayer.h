#ifndef __ESPPLAYER_H__
#define __ESPPLAYER_H__

class CEspPlayer
{
public:
	CEspPlayer();
	~CEspPlayer();

	void SetTimestamp(float fTimestamp) { m_fTimestamp = fTimestamp; }
	float GetTimestamp() { return m_fTimestamp; }

	void SetIsDormant(bool bIsDormant) { m_bIsDormant = bIsDormant; }
	bool GetIsDormant() { return m_bIsDormant; }
private:
	float m_fTimestamp;
	bool m_bIsDormant;
};

#endif // __ESPPLAYER_H__