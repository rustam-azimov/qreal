#include "logicalModelAssistApi.h"
#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace models::details;

LogicalModelAssistApi::LogicalModelAssistApi(LogicalModel &logicalModel, EditorManagerInterface const &editorManagerInterface)
	: mModelsAssistApi(logicalModel, editorManagerInterface)
	, mLogicalModel(logicalModel), mEditorManager(editorManagerInterface)
{
}

EditorManagerInterface const &LogicalModelAssistApi::editorManagerInterface() const
{
	return mModelsAssistApi.editorManagerInterface();
}

qrRepo::LogicalRepoApi const &LogicalModelAssistApi::logicalRepoApi() const
{
	return mLogicalModel.api();
}

qrRepo::LogicalRepoApi &LogicalModelAssistApi::mutableLogicalRepoApi()
{
	return mLogicalModel.mutableApi();
}

Id LogicalModelAssistApi::createElement(Id const &parent, Id const &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	Id const newElementId(type, QUuid::createUuid().toString());
	QString const elementFriendlyName = mModelsAssistApi.editorManagerInterface().friendlyName(type);
	mLogicalModel.addElementToModel(parent, newElementId, Id(), elementFriendlyName, QPointF(0, 0));
	return newElementId;
}

Id LogicalModelAssistApi::createElement(Id const &parent, Id const &id
		, bool isFromLogicalModel, QString const &name
		, QPointF const &position, Id const &preferedLogicalId)
{
	Q_UNUSED(preferedLogicalId)
	return mModelsAssistApi.createElement(parent, id, id, isFromLogicalModel, name, position);
}

void LogicalModelAssistApi::stackBefore(const Id &element, const Id &sibling)
{
	mModelsAssistApi.stackBefore(element, sibling);
}

IdList LogicalModelAssistApi::children(Id const &element) const
{
	return mLogicalModel.api().children(element);
}

void LogicalModelAssistApi::changeParent(Id const &element, Id const &parent, QPointF const &position)
{
	Q_UNUSED(position);
	mLogicalModel.changeParent(mLogicalModel.indexById(element), mLogicalModel.indexById(parent), QPointF());
}

void LogicalModelAssistApi::connect(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().connect(source, destination);
}

void LogicalModelAssistApi::disconnect(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().disconnect(source, destination);
}

void LogicalModelAssistApi::addUsage(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().addUsage(source, destination);
}

void LogicalModelAssistApi::deleteUsage(Id const &source, Id const &destination)
{
	mLogicalModel.mutableApi().deleteUsage(source, destination);
}

Id LogicalModelAssistApi::createConnectedElement(Id const &source, Id const &elementType)
{
	Id element = createElement(Id::rootId(), elementType);
	QString sourceName = mLogicalModel.data(mLogicalModel.indexById(source), Qt::DisplayRole).toString();
	QString typeName = editorManagerInterface().friendlyName(elementType);
	mLogicalModel.setData(mLogicalModel.indexById(element), sourceName + " " + typeName, Qt::DisplayRole);
	return element;
}

void LogicalModelAssistApi::createConnected(Id const &sourceElement, Id const &elementType)
{
	Id element = createConnectedElement(sourceElement, elementType);
	connect(sourceElement, element);
}

void LogicalModelAssistApi::createUsed(Id const &sourceElement, Id const &elementType)
{
	Id element = createConnectedElement(sourceElement, elementType);
	addUsage(sourceElement, element);
}

IdList LogicalModelAssistApi::diagramsFromList(IdList const &list) const
{
	// TODO: diagrams are kinda special, so we need the editor to be able to
	// tell us whether this particular element is a diagram or not
	IdList result;
	foreach (Id type, list) {
		if (type.element().split("_").back().contains("Diagram", Qt::CaseInsensitive)) {
			if (!result.contains(type))
				result.append(type);
		}
	}
	return result;
}

IdList LogicalModelAssistApi::diagramsAbleToBeConnectedTo(Id const &element) const
{
	return diagramsFromList(editorManagerInterface().connectedTypes(element.type()));
}

IdList LogicalModelAssistApi::diagramsAbleToBeUsedIn(Id const &element) const
{
	return diagramsFromList(editorManagerInterface().usedTypes(element.type()));
}

QVariant LogicalModelAssistApi::property(Id const &id, QString const &name) const
{
	return mLogicalModel.mutableApi().property(id, name);
}

void LogicalModelAssistApi::setProperty(Id const &id, QString const &name, QVariant const &value)
{
	mLogicalModel.mutableApi().setProperty(id, name, value);
}

void LogicalModelAssistApi::setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName)
{
	int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return;
	mModelsAssistApi.setProperty(elem, newValue, roleIndex);
}

QVariant LogicalModelAssistApi::propertyByRoleName(Id const &elem, QString const &roleName) const
{
	int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return "";
	return mModelsAssistApi.property(elem, roleIndex);
}

bool LogicalModelAssistApi::isLogicalId(Id const &id) const
{
	return mModelsAssistApi.indexById(id) != QModelIndex();
}

void LogicalModelAssistApi::setTo(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setTo(elem, newValue);
}

Id LogicalModelAssistApi::to(Id const &elem) const
{
	return mModelsAssistApi.to(elem);
}

void LogicalModelAssistApi::setFrom(Id const &elem, Id const &newValue)
{
	mModelsAssistApi.setFrom(elem, newValue);
}

Id LogicalModelAssistApi::from(Id const &elem) const
{
	return mModelsAssistApi.from(elem);
}

QModelIndex LogicalModelAssistApi::indexById(Id const &id) const
{
	return mModelsAssistApi.indexById(id);
}

Id LogicalModelAssistApi::idByIndex(QModelIndex const &index) const
{
	return mModelsAssistApi.idByIndex(index);
}

QPersistentModelIndex LogicalModelAssistApi::rootIndex() const
{
	return mModelsAssistApi.rootIndex();
}

Id LogicalModelAssistApi::rootId() const
{
	return mModelsAssistApi.rootId();
}

bool LogicalModelAssistApi::hasRootDiagrams() const
{
	return mModelsAssistApi.hasRootDiagrams();
}

int LogicalModelAssistApi::childrenOfRootDiagram() const
{
	return mModelsAssistApi.childrenOfRootDiagram();
}

int LogicalModelAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModelsAssistApi.childrenOfDiagram(parent);
}

void LogicalModelAssistApi::removeReferencesTo(Id const &id)
{
	IdList backReferences = mLogicalModel.api().property(id, "backReferences").value<IdList>();

	foreach (Id const &reference, backReferences) {
		mLogicalModel.api().removeBackReference(id, reference);
		removeReference(reference, id);
	}
}

void LogicalModelAssistApi::removeReferencesFrom(Id const &id)
{
	QStringList referenceProperties = mEditorManager.referenceProperties(id.type());

	foreach (QString const &property, referenceProperties) {
		QString propertyString = mLogicalModel.api().property(id, property).toString();
		if (!propertyString.isEmpty()) {
			Id propertyValue = Id::loadFromString(propertyString);
			mLogicalModel.api().removeBackReference(propertyValue, id);
		}
	}
}

void LogicalModelAssistApi::removeReference(Id const &id, Id const &reference)
{
	QStringList referenceProperties = mEditorManager.referenceProperties(id.type());

	foreach (QString const &propertyName, referenceProperties) {
		QString stringData = mLogicalModel.api().property(id, propertyName).toString();
		if (stringData == reference.toString()) {
			mLogicalModel.mutableApi().setProperty(id, propertyName, "");
		}
	}
}

void LogicalModelAssistApi::removeElement(Id const &logicalId)
{
	QPersistentModelIndex const index = indexById(logicalId);
	if (logicalRepoApi().exist(logicalId) && index.isValid()) {
		removeReferencesTo(logicalId);
		mLogicalModel.removeRow(index.row(), index.parent());
	}
}
