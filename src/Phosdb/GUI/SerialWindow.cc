#include "SerialWindow.hh"

#include <Clients/TcpClient.hh>

namespace Phosdb::GUI {
template class SerialWindow<Phosdb::Clients::TcpClient>;
template SerialWindow<Phosdb::Clients::TcpClient>::SerialWindow(
	Phosdb::Clients::SerialClient<Phosdb::Clients::TcpClient> &Serial
);

template<typename T> SerialWindow<T>::SerialWindow(
	Phosdb::Clients::SerialClient<T> &Serial
) : Serial(Serial) { }

template<> VOID SerialWindow<Phosdb::Clients::TcpClient>::RenderClientInfo() {
	Phosdb::Clients::TcpClient &Tcp = Serial.GetInnerClient();

	ImGui::LabelText("Address", "%s", Tcp.GetAddress().c_str());
	ImGui::LabelText("Port", "%hu", Tcp.GetPort());
}

template<typename T> VOID SerialWindow<T>::Update() {
	if (!CanUpdate)
		return;

	std::vector<std::vector<UINT8>> Packets;
	
	if (!Serial.ReceivePackets(Packets))
		CanUpdate = FALSE;

	if (Packets.empty())
		return;

	std::vector<std::string> ParsedPackets;
	
	Serial.ParsePackets(Packets, ParsedPackets);

	for (std::string Packet : ParsedPackets)
		Log.append(Packet);
}

template<typename T> VOID SerialWindow<T>::Render() {
	if (ImGui::Begin("Serial")) {
		if (ImGui::BeginTable("", 2)) {
			if (ImGui::TableNextColumn()) {
				ImDrawList *DrawList = ImGui::GetWindowDrawList();

				ImGui::SeparatorText("Status");

				BOOL Connected = Serial.IsConnected();
				ImU32 Color = Connected ?
					IM_COL32(0x00, 0xFF, 0x00, 0xFF) :
					IM_COL32(0xFF, 0x00, 0x00, 0xFF);

				ImGui::PushStyleColor(ImGuiCol_Text, Color);

				ImVec2 Position = ImGui::GetCursorScreenPos();

				Position.x += 4;
				Position.y += 5;

				ImGui::LabelText("Status", Connected ? EMSP EMSP "Connected" : EMSP EMSP "Disconnected");
				DrawList->AddTriangleFilled(
					ImVec2(Position.x,      Position.y + 10),
					ImVec2(Position.x + 5,  Position.y),
					ImVec2(Position.x + 10, Position.y + 10),
					Color);

				ImGui::PopStyleColor();

				RenderClientInfo();

				ImGui::BeginDisabled(Serial.IsConnected());
				// This doesn't actually try to create a new socket and reconnect yet
				if (ImGui::Button("Reconnect"))
					CanUpdate = TRUE;
				ImGui::EndDisabled();

				ImGui::SeparatorText("Output");

				ImGui::TextWrapped("A message packet will be sent to COM1 containing the following text.");
				ImGui::InputText("Message", &Message);
				
				ImGui::BeginDisabled(!Serial.IsConnected());
				if (ImGui::Button("Send"))
					Serial.SendMsg(Message);
				ImGui::EndDisabled();
			}

			if (ImGui::TableNextColumn()) {
				ImGui::SeparatorText("Input");
				ImGui::TextWrapped("Packets read from COM1.");
				ImGui::InputTextMultiline("##Log", &Log, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
			}

			ImGui::EndTable();
		}
	}

	ImGui::End();
}
}