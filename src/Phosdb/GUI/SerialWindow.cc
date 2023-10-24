#include "SerialWindow.hh"

namespace Phosdb::GUI {
SerialWindow::SerialWindow(
	const Phosdb::TcpClient &Client
) {
	this->Client = &Client;
}

VOID SerialWindow::Render() {
	if (ImGui::Begin("Serial")) {
		if (ImGui::BeginTable("", 2)) {
			if (ImGui::TableNextColumn()) {
				ImDrawList *DrawList = ImGui::GetWindowDrawList();

				ImGui::SeparatorText("Socket");

				BOOL Connected = Client->IsConnected();
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

				ImGui::LabelText("Address", "%s", Client->GetAddress().c_str());
				ImGui::LabelText("Port", "%hu", Client->GetPort());

				ImGui::SeparatorText("Input");

				ImGui::TextWrapped("A message packet will be sent to COM1 containing the following text.");
				ImGui::InputText("Message", &Message);
				ImGui::Button("Send");
			}

			if (ImGui::TableNextColumn()) {
				ImGui::SeparatorText("Output");
				ImGui::TextWrapped("Packets read from COM1.");
				ImGui::InputTextMultiline("##Output", &Output, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
			}

			ImGui::EndTable();
		}
	}

	ImGui::End();
}
}