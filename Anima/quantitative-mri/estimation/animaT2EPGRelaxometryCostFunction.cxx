#include "animaT2EPGRelaxometryCostFunction.h"
#include <animaEPGSignalSimulator.h>

namespace anima
{
    
T2EPGRelaxometryCostFunction::MeasureType
T2EPGRelaxometryCostFunction::GetValue(const ParametersType & parameters) const
{
    m_T2Value = parameters[0];
    m_M0Value = parameters[1];

    double residualValue = 0;
    unsigned int numT2Signals = m_T2RelaxometrySignals.size();

    anima::EPGSignalSimulator t2SignalSimulator;
    t2SignalSimulator.SetB1OnExcitationAngle(m_B1OnExcitationAngle);
    t2SignalSimulator.SetNumberOfEchoes(m_T2RelaxometrySignals.size());
    t2SignalSimulator.SetEchoSpacing(m_T2EchoSpacing);
    t2SignalSimulator.SetExcitationFlipAngle(m_T2ExcitationFlipAngle);
    t2SignalSimulator.SetFlipAngle(m_T2FlipAngles[0]);

    anima::EPGSignalSimulator::RealVectorType simulatedT2Values = t2SignalSimulator.GetValue(m_T1Value,m_T2Value,m_B1Value,m_M0Value);

    for (unsigned int i = 0;i < numT2Signals;++i)
        residualValue += (simulatedT2Values[i] - m_T2RelaxometrySignals[i]) * (simulatedT2Values[i] - m_T2RelaxometrySignals[i]);

    return residualValue;
}
    
} // end namespace anima
