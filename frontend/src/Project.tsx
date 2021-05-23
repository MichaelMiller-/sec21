import React, { useEffect, useState } from "react";
import { Button, ButtonGroup, Collapse, Form, Modal, Row, Spinner } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import { BackButton } from "./ui/BackButton";
import { CrossSectionList } from "./CrossSectionList";
import { DbProject } from "./DatabaseInterface";
import { MaterialList } from "./MaterialList"
import axios from 'axios';

export type ProjectListProps = {
    projectId: string
}

export default function Project(props: ProjectListProps) {
    
    const history = useHistory();

    const [project, setProject] = useState<DbProject>({
        id: 0,
        name: "",
        title: ""
    });
    const [showMaterialList, setVisibilityMaterialList] = useState(false);
    const [showCrossSectionList, setVisibilityCrossSectionList] = useState(false);
    const [showTasklistModal, setVisibilityTasklistModal] = useState(false);
    const [loading, setProjectLoading] = useState(true)

    useEffect(() => { getProject() }, []);

    async function getProject() {
        await axios.get(process.env.BACKEND + '/project/' + props.projectId)
            .then(response => setProject(response.data))
            .finally(() => setProjectLoading(false))
    }

    const handleShow = () => setVisibilityTasklistModal(true);

    const onDelete = (id: number) => {
        console.log("on delete: ", id);
    };
    const onBack = () => { history.push("/"); }
    const onStructure = () => { history.push("/structure/" + props.projectId); }

    if (loading) {
        return (<><Spinner animation="border" role="status"></Spinner></>);
    }

    //! \todo show list of tasks
    return (
        <>
            <BackButton onClick={onBack} disabled={false} />
            <h2>Project: {project.name}</h2>
            <>{project.title}</>
            <Row>
                <ButtonGroup vertical>
                    <Button onClick={onStructure}>Structure</Button>
                    <Button onClick={() => setVisibilityMaterialList(!showMaterialList)} aria-controls="example-collapse-text" aria-expanded={showMaterialList}>
                        Materials
                </Button>
                    <Collapse in={showMaterialList}>
                        <div id="example-collapse-text">
                            <MaterialList projectId={props.projectId} />
                        </div>
                    </Collapse>
                    <Button onClick={() => setVisibilityCrossSectionList(!showCrossSectionList)} aria-controls="cross-section-list" aria-expanded={showCrossSectionList}>
                        Cross Sections
                </Button>
                    <Collapse in={showCrossSectionList}>
                        <div id="cross-section-list">
                            <CrossSectionList projectId={props.projectId} />
                        </div>
                    </Collapse>
                </ButtonGroup>
            </Row>
            <Button onClick={handleShow}>New task...</Button>
            <Modal centered keyboard={true} show={showTasklistModal}>
                <Modal.Header closeButton>
                    <Modal.Title>Login Form</Modal.Title>
                </Modal.Header>
                <Modal.Body>
                    <></>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary">Close Modal</Button>
                </Modal.Footer>
            </Modal>
        </>
    );
}

export interface RouterComponentProjectProps extends RouteComponentProps<ProjectListProps> { }
