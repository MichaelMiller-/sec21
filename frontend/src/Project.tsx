import React, {useEffect, useState} from "react";
import {Button, Spinner, Tab, Tabs} from "react-bootstrap";
import {RouteComponentProps, useHistory} from "react-router-dom";
import CrossSectionList from './CrossSectionList'
import {
    DbCrossSection,
    DbCurveMember, DbLoadCase,
    DbMaterial, DbPointAction,
    DbPointSupport,
    DbProject,
    DbStructuralPoint, DbSurfaceMember
} from './DatabaseInterface'
import MaterialList from './MaterialList'
import axios from 'axios'
import Header from "./ui/Header";
import StructuralPointsList from "./StructuralPointsList";
import LoadCaseList from "./LoadCaseList";
import SurfaceMemberList from "./SurfaceMemberList";
import CurveMemberList from "./CurveMemberList";
import PointSupportList from "./PointSupportList";
import PointActionList from "./PointActionList";
import LogPalette from "./LogPalette";
import Scene from "./Scene";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faSync} from "@fortawesome/free-solid-svg-icons";

export interface ListProps<T> {
    projectId: string
    items: Array<T>
    onUpdate: () => void
}

type ProjectListProps = {
    projectId: string
}


export const Project = (props: ProjectListProps) => {
    const history = useHistory();

    const [project, setProject] = useState<DbProject>({
        id: 0,
        name: "",
        title: "",
        createdAt: new Date(),
        modifiedAt: new Date()
    })
    const [structuralPoints, setStructuralPoints] = useState<DbStructuralPoint[]>([])
    const [pointSupports, setPointSupports] = useState<DbPointSupport[]>([])
    const [materials, setMaterials] = useState<DbMaterial[]>([])
    const [crossSections, setCrossSections] = useState<DbCrossSection[]>([])
    const [curveMembers, setCurveMembers] = useState<DbCurveMember[]>([])
    const [surfaceMembers, setSurfaceMembers] = useState<DbSurfaceMember[]>([])
    const [loadCases, setLoadCases] = useState<DbLoadCase[]>([])
    const [pointActions, setPointActions] = useState<DbPointAction[]>([])
    const [loading, setLoading] = useState(true)
    const [disablePointSupportTab, setDisablePointSupportTab] = useState(true)
    const [disableCrossSectionTab, setDisableCrossSectionTab] = useState(true)
    const [disableCurveMemberTab, setDisableCurveMemberTab] = useState(true)
    const [disableLoadCaseTab, setDisableLoadCaseTab] = useState(true)
    const [disableSurfaceTab, setDisableSurfaceTab] = useState(true)
    const [calculated, setCalculated] = useState(false)
    const [showLog, setShowLog] = useState(false)
    const [errors, setErrors] = useState<string[]>([])
    const [warnings, setWarnings] = useState<string[]>([])

    const getPointSupportList = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/pointsupports/' + props.projectId)
            .then(response => {
                setPointSupports(response.data)
            })
    }
    const getStructuralPointList = async () => {
        await axios.get(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId)
            .then(response => {
                setStructuralPoints(response.data)
                setDisableCurveMemberTab(response.data.length < 2)
                setDisableSurfaceTab(response.data.length === 0)
                setDisablePointSupportTab(response.data.length === 0)
            })
    }
    const getMaterialList = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/materials/' + props.projectId)
            .then(response => {
                setMaterials(response.data)
                setDisableCrossSectionTab(response.data.length === 0)
                setDisableSurfaceTab(response.data.length === 0)
            })
    }
    const getCrossSectionList = async () => {
        await axios.get(process.env.REACT_APP_BACKEND + '/crosssections/' + props.projectId)
            .then(response => {
                setCrossSections(response.data)
                // setDisableCurveMemberTab(response.data.length === 0 || structuralPoints.length < 2)
                if (structuralPoints.length > 1) {
                    setDisableCurveMemberTab(response.data.length === 0 || structuralPoints.length < 2)
                }
            })
    }
    const getCurveMemberList = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/curvemembers/' + props.projectId)
            .then(response => {
                setCurveMembers(response.data)
                setDisableLoadCaseTab(response.data.length === 0)
            })
    }
    const getSurfaceMemberList = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/surfacemembers/' + props.projectId)
            .then(response => {
                setSurfaceMembers(response.data)
            })
    }
    const getLoadCaseList = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/loadcases/' + props.projectId)
            .then(response => {
                setLoadCases(response.data)
            })
    }
    const getPointActionList = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/pointactions/' + props.projectId)
            .then(response => {
                setPointActions(response.data)
            })
    }
    const pushError = (msg: string) => {
        errors.push(msg)
        setShowLog(true)
    }
    const onDeleteMaterial = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/material/' + id);
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getMaterialList()
        }
    }
    const onDeleteCrossSection = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/crosssection/' + id);
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getCrossSectionList()
        }
    }
    const onDeleteStructuralPoint = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/structuralpoint/' + id);
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getStructuralPointList()
        }
    }
    const onDeletePointSupport = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/pointsupport/' + id);
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getPointSupportList()
        }
    }
    const onDeleteCurveMember = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/curvemember/' + id);
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getCurveMemberList()
        }
    }
    const onDeleteSurfaceMember = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/surfacemember/' + id);
        console.log(response.data)
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getSurfaceMemberList()
        }
    }
    const onDeleteLoadCase = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/loadcase/' + id);
        console.log(response.data)
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getLoadCaseList()
        }
    }
    const onDeletePointAction = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/pointaction/' + id);
        console.log(response.data)
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getPointActionList()
        }
    }

    useEffect(() => {
        getStructuralPointList()
        getMaterialList()
        getCrossSectionList()
        getCurveMemberList()
        getPointSupportList()
        getSurfaceMemberList()
        getPointActionList()
        getLoadCaseList()

        axios.get(process.env.REACT_APP_BACKEND + '/project/' + props.projectId)
            .then(response => {
                setProject(response.data)
            })
            .finally(() => setLoading(false))

        axios.get(process.env.REACT_APP_BACKEND + '/pointsupports/' + props.projectId)
            .then(response => {
                setPointSupports(response.data)
            })

        setDisableCurveMemberTab(structuralPoints.length < 2)
        // eslint-disable-next-line
    }, [props.projectId]);

    const runCalculation = async () => {
        //! \todo hardcoded loadcase
        const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/esbp?project=' + props.projectId + '&loadcase=1')

        setCalculated(response.data[0].status)

        const messages = response.data[0].messages.filter((e: { kind: string; }) => e.kind === "Error").map((e: any) => e.message)
        if (messages.length > 0) {
            setShowLog(true)
            setErrors(messages)
        }
    }

    const openReport = async () => {
        const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/generate_report', {params: {project: props.projectId}});
        const filename = response.data[0].data;
        window.open(process.env.REACT_APP_DOWNLOAD_URL + filename, "_blank", "toolbar=yes,width=600,height=800");
    }

    const onCheck = async () => {
        const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/sanity_check', {params: {project: props.projectId}});

        const em = response.data[0].messages.filter((e: { kind: string; }) => e.kind === "Error").map((e: any) => e.message)
        const wm = response.data[0].messages.filter((e: { kind: string; }) => e.kind === "Warning").map((e: any) => e.message)

        setErrors(em)
        setWarnings(wm)
        setShowLog(em.length > 0 || wm.length > 0)
    }

    const onBack = () => {
        history.push("/");
    }

    if (loading) {
        return (<><Spinner animation="border" role="status"></Spinner></>);
    }

    //! \TODO: enable 'calculate' only if at least one loadcase is selected
    return (
        <>
            <Header onBack={onBack} disabledBackButton={false} title={"Project: " + project.name}/>
            <Tabs defaultActiveKey="system" id="uncontrolled-tab-example" className="mb-3">
                <Tab eventKey="system" title="System">
                    <Button variant="link" onClick={() => {
                            getPointSupportList()
                            getCurveMemberList()
                        }}>
                        <FontAwesomeIcon icon={faSync} size="2x" />
                    </Button>
                    <Button onClick={onCheck} disabled={structuralPoints.length < 2}>Check</Button>
                    <Button onClick={runCalculation}>Calculate</Button>
                    <Button disabled={!calculated} onClick={openReport}>Report</Button>
                    <Scene spts={structuralPoints} members={curveMembers}/>
                </Tab>
                <Tab eventKey="structure"  title="Structure">
                    <StructuralPointsList projectId={props.projectId} items={structuralPoints}
                                          onUpdate={getStructuralPointList} onDelete={onDeleteStructuralPoint}/>
                </Tab>
                <Tab eventKey="pointSupport" title="Point Support" disabled={disablePointSupportTab}>
                    <PointSupportList projectId={props.projectId} items={pointSupports} onUpdate={getPointSupportList} onDelete={onDeletePointSupport}/>
                </Tab>
                <Tab eventKey="materials" title="Materials">
                    <MaterialList projectId={props.projectId} items={materials} onUpdate={getMaterialList} onDelete={onDeleteMaterial}/>
                </Tab>
                <Tab eventKey="crossSections" title="Cross Sections" disabled={disableCrossSectionTab}>
                    <CrossSectionList projectId={props.projectId} items={crossSections}
                                      onUpdate={getCrossSectionList} onDelete={onDeleteCrossSection}/>
                </Tab>
                <Tab eventKey="members" title="Curve Members" disabled={disableCurveMemberTab}>
                    <CurveMemberList projectId={props.projectId} items={curveMembers} onUpdate={getCurveMemberList} onDelete={onDeleteCurveMember}/>
                </Tab>
                <Tab eventKey="surface" title="Surfaces" disabled={disableSurfaceTab}>
                    <SurfaceMemberList projectId={props.projectId} items={surfaceMembers}
                                       onUpdate={getSurfaceMemberList} onDelete={onDeleteSurfaceMember}/>
                </Tab>
                <Tab eventKey="loadcases" title="Loadcases" disabled={disableLoadCaseTab}>
                    <LoadCaseList projectId={props.projectId} items={loadCases} onUpdate={getLoadCaseList} onDelete={onDeleteLoadCase}/>
                </Tab>
                <Tab eventKey="loads" title="Loads" disabled={false}>
                    <PointActionList projectId={props.projectId} items={pointActions} onUpdate={getPointActionList} onDelete={onDeletePointAction} />
                </Tab>
            </Tabs>

            <LogPalette show={showLog} errorMessages={errors} warningMessages={warnings} onHide={() => {
                setShowLog(false)
                setErrors([])
                setWarnings([])
            }}/>
        </>
    );
}

export interface RouterComponentProjectProps extends RouteComponentProps<ProjectListProps> {
}
